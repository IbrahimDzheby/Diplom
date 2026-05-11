clc; clear; close all;

%% Параметры авто
m = 1600;
r = 0.285;

Cr = 0.015;
Cd = 0.32;
A  = 2.2;
rho = 1.225;
g = 9.81;

GP_ratio = 5.4;

GP_eff   = 0.95;
Gimbal_eff = 0.98;
WheelRed_eff = 0.99;

Tsmn_eff = GP_eff * Gimbal_eff * WheelRed_eff;

J_w = 7.0;      % кг*м², колёса + полуоси
J_t = 1.0;      % кг*м², трансмиссия до вариатора
J_e = 0.2;      % кг*м², двигатель

idle_fuel = 0.6 / 3600; % ~0.6 л/час → л/с

%% Диапазон оборотов и предельный момент ДВС

rpm_grid = linspace(600, 5400, 50);

rpm_tmax_curve = [600.00
941.88
1488.89
1926.50
2350.43
3006.84
3252.99
3663.25
4100.85
4497.44
4839.32
5167.52
5400.00];

torque_tmax_curve = [101.10
106.59
116.48
123.08
131.32
136.81
138.46
141.76
142.86
140.11
137.91
136.26
134.07];

Tmax_curve = @(rpm) interp1(rpm_tmax_curve, torque_tmax_curve, rpm, 'linear', 'extrap');

%% CVT карта

% загрузка данных из таблицы
cvt_data = readtable('CVT.xlsx', 'VariableNamingRule', 'preserve');
cvt_ratio_all = cvt_data{:,1};      % передаточное отношение
cvt_torque_all = cvt_data{:,2};     % момент
cvt_eta_all = cvt_data{:,3};        % КПД

cvt_ratio_vec = unique(cvt_ratio_all, 'stable');
cvt_torque_vec = unique(cvt_torque_all, 'stable');
cvt_eta_grid = reshape(cvt_eta_all, length(cvt_torque_vec), length(cvt_ratio_vec))';   % 22×19

cvt_eta_surf = griddedInterpolant({cvt_ratio_vec, cvt_torque_vec}, cvt_eta_grid, 'linear', 'none');

%% WLTP цикл
data = readtable('WLTP.xlsx');

t = data.Time;        % [s]
v = data.Speed / 3.6; % [m/s]

dt = [0; diff(t)];

%% Основной цикл
fuel_flow = zeros(size(t));
rpm_points = [];
torque_points = [];

for k = 2:length(t)

    % --- скорость и ускорение
    vk = v(k);

    if vk == 0
        fuel_flow(k) = idle_fuel * 745; % в г/с
        continue;
    end

    ak = (v(k) - v(k-1)) / dt(k);

    % --- угловые скорость и ускорение 
    wk = vk / r; % рад/с
    ek = ak / r; % рад/с2

    % --- силы
    F_roll = m * g * Cr;
    F_aero = 0.5 * rho * Cd * A * vk^2;
    F_iner = m * ak;

    F_total = F_roll + F_aero + F_iner;

    if F_total <= 0
        fuel_flow(k) = idle_fuel * 745; % в г/с
        continue;
    end

    % --- момент на колесах
    T_wheel = F_total * r + J_w * ek;

    best_fuel = inf;

    valid_count = 0;
    cvt_valid_flag = 0;
    rpm_best = 0;
    T_best = 0;

    % ПЕРЕБОР ОБОРОТОВ ДВС
    for rpm = rpm_grid

        w_eng = rpm * 2*pi/60;

        % передаточное число CVT
        i_cvt = w_eng / (wk * GP_ratio);

        % ограничения
        if i_cvt < 0.427
            i_cvt = 0.427;
        elseif i_cvt > 2.561
            i_cvt = 2.561;
        end
        
        T_cvt_out = T_wheel / (GP_ratio * Tsmn_eff) + J_t * ek * GP_ratio;

        if T_cvt_out <= 0
            continue;
        end
    
        CVT_eff = CVT_map(i_cvt, T_cvt_out, cvt_eta_surf, cvt_torque_vec);
        if isnan(CVT_eff)
            cvt_valid_flag = 1;
            continue;
        end

        T_eng = T_cvt_out / (i_cvt * CVT_eff) + J_e * ek * GP_ratio * i_cvt;

        if T_eng > Tmax_curve(rpm) 
            continue;
        elseif T_eng <= 0
            continue;
        end

        % BSFC
        bsfc = BSFC(rpm, T_eng); % [г/кВт*ч]

        % мощность двигателя
        P_eng = T_eng * w_eng / 1000; % [кВт]

        % расход [г/с]
        fuel = bsfc * P_eng / 3600;

        if fuel < best_fuel
            best_fuel = fuel;
            rpm_best = rpm;
            T_best = T_eng;
        end

        valid_count = 1;
    end

    if valid_count == 0
        if cvt_valid_flag == 1
            fprintf('CVT map error на шаге %d\n', k);
        end
    end

    rpm_points(end+1) = rpm_best;
    torque_points(end+1) = T_best;

    if isinf(best_fuel)
        fuel_flow(k) = fuel_flow(k-1); % или 0
    else
        fuel_flow(k) = best_fuel;
    end

end

figure;
plot(t, fuel_flow);
xlabel('t [s]');
ylabel('Fuel [g/s]');
title('Fuel flow');
grid on;

figure;
scatter(rpm_points, torque_points, 10, 'filled');
grid on;

xlabel('Engine speed (RPM)');
ylabel('Torque (Nm)');
title('Рабочие точки двигателя');

xlim([0 6000]);
ylim([0 150]);

%% Интеграция
fuel_total = sum(fuel_flow .* dt); % [г]

distance = trapz(t, v); % [м]

% перевод в л/100 км (бензин ~745 г/л)
fuel_l = fuel_total / 745;

result = fuel_l / (distance / 100000);

fprintf('Расход: %.2f л/100км\n', result);

%% Значение потерь в вариаторе

function eta = CVT_map(i_val, T_out_val, cvt_eta_surf, cvt_torque_vec)
    cost = @(T_in) i_val * T_in * cvt_eta_surf(i_val, T_in) - T_out_val;
    options = optimset('Display','off');
    try
        T_sol = fzero(cost, [min(cvt_torque_vec), max(cvt_torque_vec)], options);
        eta = cvt_eta_surf(i_val, T_sol);
    catch
        eta = NaN;
    end
end

%% Значение потерь в двигателе

function bsfc = BSFC(rpm, torque)
    % BSFC интерполяция по триангулированной поверхности
    % Получаем обороты и момент -> находим треугольник -> интерполируем
    
    persistent vertices triangles
    persistent rpm_min rpm_max torque_min torque_max
    persistent nTri
    
    % Загружаем данные при первом вызове
    if isempty(vertices)
        S = coder.load('BSFC_surface.mat');
        vertices = S.all_vertices;
        triangles = S.all_triangles;
        
        rpm_min = min(vertices(:,1));
        rpm_max = max(vertices(:,1));
        torque_min = min(vertices(:,2));
        torque_max = max(vertices(:,2));
        
        nTri = size(triangles, 1);
    end
    
    % Проверяем, что точка внутри области данных
    if rpm < rpm_min || rpm > rpm_max || torque < torque_min || torque > torque_max
        % Точка вне области - возвращаем NaN или можно задать граничное значение
        bsfc = NaN;
        return;
    end
    
    % Ищем треугольник, содержащий точку
    for t = 1:nTri
        % Получаем вершины треугольника
        i1 = triangles(t, 1);
        i2 = triangles(t, 2);
        i3 = triangles(t, 3);
        
        p1 = vertices(i1, :);
        p2 = vertices(i2, :);
        p3 = vertices(i3, :);
        
        % Проверяем, находится ли точка внутри треугольника
        if pointInTriangle([rpm, torque], p1(1:2), p2(1:2), p3(1:2))
            % Точка внутри - делаем барицентрическую интерполяцию
            bsfc = barycentricInterp(p1, p2, p3, [rpm, torque]);
            return;
        end
    end
    
    % Если не нашли треугольник (точка между контурами, но вне треугольников)
    % Такое возможно на границах между уровнями
    bsfc = NaN;
end

function inside = pointInTriangle(pt, p1, p2, p3)
    % Проверка, находится ли точка внутри треугольника
    % pt, p1, p2, p3 - точки [x y]
    
    % Вычисляем барицентрические координаты
    v0 = p3 - p1;
    v1 = p2 - p1;
    v2 = pt - p1;
    
    dot00 = v0(1)*v0(1) + v0(2)*v0(2);
    dot01 = v0(1)*v1(1) + v0(2)*v1(2);
    dot02 = v0(1)*v2(1) + v0(2)*v2(2);
    dot11 = v1(1)*v1(1) + v1(2)*v1(2);
    dot12 = v1(1)*v2(1) + v1(2)*v2(2);
    
    invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    
    % Проверка: точка внутри, если u >= 0, v >= 0 и u + v <= 1
    inside = (u >= 0) && (v >= 0) && (u + v <= 1);
end

function z = barycentricInterp(p1, p2, p3, pt)
    % Барицентрическая интерполяция для получения Z
    % p1, p2, p3 - вершины треугольника [x y z]
    % pt - точка [x y]
    
    % Вычисляем барицентрические координаты
    v0 = p3(1:2) - p1(1:2);
    v1 = p2(1:2) - p1(1:2);
    v2 = pt - p1(1:2);
    
    dot00 = v0(1)*v0(1) + v0(2)*v0(2);
    dot01 = v0(1)*v1(1) + v0(2)*v1(2);
    dot02 = v0(1)*v2(1) + v0(2)*v2(2);
    dot11 = v1(1)*v1(1) + v1(2)*v1(2);
    dot12 = v1(1)*v2(1) + v1(2)*v2(2);
    
    invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    w = 1 - u - v;
    
    % Интерполяция Z
    z = w * p1(3) + v * p2(3) + u * p3(3);
end