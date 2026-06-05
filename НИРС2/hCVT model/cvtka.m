clc; clear; close all;

%% Параметры авто
m = 1250;
r = 0.285;

Cr = 0.012;
Cd = 0.31;
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

rpm_grid = linspace(800, 5200, 50);

rpm_tmax_curve = [800
    1113.39
    1614.81
    2015.95
    2404.56
    3006.27
    3231.91
    3607.98
    4009.12
    4372.65
    4686.04
    4986.89
    5200];

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
data = readtable('WLTP_city.xlsx');

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
        % fuel_flow(k) = idle_fuel * 745; % в г/с
        fuel_flow(k) = 0;
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

% figure;
% plot(t, fuel_flow);
% xlabel('t [s]');
% ylabel('Fuel [g/s]');
% title('Fuel flow');
% grid on;

figure;
plot(t, fuel_flow, 'b-', 'LineWidth', 1.5);
ylabel('Расход топлива, г/с');
xlabel('Время, с');
grid on;

% figure;
% scatter(rpm_points, torque_points, 10, 'filled');
% grid on;
% 
% xlabel('Engine speed (RPM)');
% ylabel('Torque (Nm)');
% title('Рабочие точки двигателя');
% 
% xlim([0 6000]);
% ylim([0 150]);

% Изолинии BSFC из Excel (три колонки: rpm, torque, bsfc)
if exist('BSFC.xlsx', 'file')
    bsfc_lines = readmatrix('BSFC.xlsx');
    rpm_lines = bsfc_lines(:, 1);
    torque_lines = bsfc_lines(:, 2);
    bsfc_values = bsfc_lines(:, 3);
else
    warning('Файл BSFC.xlsx не найден. Изолинии не будут построены.');
    rpm_lines = []; torque_lines = []; bsfc_values = [];
end

figure('Name', 'Рабочие точки ДВС на карте BSFC', 'NumberTitle', 'off',...
    'Color', 'w');
set(gcf, 'Position', [100, 100, 900, 700]);
hold on;

% 1. Изолинии BSFC (если данные загружены)
if ~isempty(rpm_lines)
    unique_bsfc = unique(bsfc_values);
    unique_bsfc = sort(unique_bsfc);
    colors = jet(length(unique_bsfc));
    
    for i = 1:length(unique_bsfc)
        level = unique_bsfc(i);
        level_idx = abs(bsfc_values - level) < 1e-6;
        rpm_level = rpm_lines(level_idx);
        torque_level = torque_lines(level_idx);
        
        if length(rpm_level) > 2
            % Сортировка точек по полярному углу для замыкания контура
            center_rpm = mean(rpm_level);
            center_torque = mean(torque_level);
            angles = atan2(torque_level - center_torque, rpm_level - ...
                center_rpm);
            [~, sort_idx] = sort(angles);
            rpm_level = rpm_level(sort_idx);
            torque_level = torque_level(sort_idx);
            rpm_level(end+1) = rpm_level(1);
            torque_level(end+1) = torque_level(1);
            plot(rpm_level, torque_level, 'Color', colors(i, :), ...
                'LineWidth', 1.5);
        elseif length(rpm_level) == 2
            plot(rpm_level, torque_level, 'Color', colors(i, :), ...
                'LineWidth', 1.5);
        end
    end
end

% 2. Кривая максимального момента (черная жирная линия)
plot(rpm_tmax_curve, torque_tmax_curve, 'k-', 'LineWidth', 3);

scatter(rpm_points, torque_points, 12, 'r', 'filled');

% Настройки графика
set(gcf, 'Units', 'inches', 'Position', [1, 1, 12, 8]);
% Настройка шрифта для осей, подписей и заголовка
set(gca, 'FontName', 'GOST type A', 'FontAngle', 'Italic', 'FontSize', 20);
xlabel('Обороты ДВС, об/мин', 'FontSize', 20);
ylabel('Крутящий момент ДВС, Н·м', 'FontSize', 20);
xlim([500 5500]);
ylim([0 150]);
grid on;
box on;

hold off;

%% Статистика BSFC по рабочим точкам ДВС (исключая холостой ход)
% Фильтр: обороты >= 800, момент >= 10 Н·м
valid_mask = (rpm_points >= 800) & (torque_points >= 10);
rpm_filtered = rpm_points(valid_mask);
torque_filtered = torque_points(valid_mask);

bsfc_valid = [];
for i = 1:length(rpm_filtered)
    bsfc_val = BSFC(rpm_filtered(i), torque_filtered(i));
    if ~isnan(bsfc_val)
        bsfc_valid(end+1) = bsfc_val;
    end
end

if ~isempty(bsfc_valid)
    bsfc_min = min(bsfc_valid);
    bsfc_max = max(bsfc_valid);
    bsfc_mean = mean(bsfc_valid);
    bsfc_std  = std(bsfc_valid);
    
    fprintf('\n========== Статистика BSFC (только нагрузочные точки) ==========\n');
    fprintf('Учтено точек: %d (из %d рабочих точек после фильтрации)\n', ...
        length(bsfc_valid), length(rpm_filtered));
    fprintf('Минимальный BSFC: %.2f г/(кВт·ч)\n', bsfc_min);
    fprintf('Максимальный BSFC: %.2f г/(кВт·ч)\n', bsfc_max);
    fprintf('Средний BSFC: %.2f г/(кВт·ч)\n', bsfc_mean);
    fprintf('Стандартное отклонение: %.2f г/(кВт·ч)\n', bsfc_std);
    fprintf('================================================================\n');
    
    % Рекомендуемый коэффициент s для ECMS (в г/(кВт·с))
    s_recommended = bsfc_mean / 3600;
    fprintf('Рекомендуемый эквивалентный фактор s = %.4f г/(кВт·с) (или %.1f г/(кВт·ч))\n', ...
        s_recommended, bsfc_mean);
else
    warning('Не найдено корректных точек BSFC для статистики.');
end

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