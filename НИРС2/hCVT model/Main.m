clc; clear; close all;

%% ========== Параметры автомобиля ==========
vehicle = struct();
vehicle.m     = 1600;
vehicle.r     = 0.285;
vehicle.Cr    = 0.015;
vehicle.Cd    = 0.32;
vehicle.A     = 2.2;
vehicle.rho   = 1.225;
vehicle.g     = 9.81;
vehicle.GP_ratio = 5.4;
vehicle.GP_eff   = 0.95;
vehicle.Gimbal_eff = 0.98;
vehicle.WheelRed_eff = 0.99;
vehicle.Tsmn_eff = vehicle.GP_eff * vehicle.Gimbal_eff * vehicle.WheelRed_eff;
vehicle.J_w = 7.0;
vehicle.J_t = 1.0;
vehicle.J_e = 0.2;

%% ========== ДВС ==========
engine = struct();
engine.rpm_min = 800;
engine.rpm_max = 5200;
engine.rpm_tmax_curve = [800, 1113.39, 1614.81, 2015.95, 2404.56, 3006.27, 3231.91, 3607.98, 4009.12, 4372.65, 4686.04, 4986.89, 5200];
engine.torque_tmax_curve = [101.10, 106.59, 116.48, 123.08, 131.32, 136.81, 138.46, 141.76, 142.86, 140.11, 137.91, 136.26, 134.07];
engine.Tmax = @(rpm) interp1(engine.rpm_tmax_curve, engine.torque_tmax_curve, rpm, 'linear', 'extrap');
engine.T_ice_min = 10;               % минимальный момент ДВС при подаче топлива (Н·м) – исключает EV
engine.T_brake = @(rpm) max(-50, -0.02 * rpm);  % тормозной момент ДВС при отсечке (Н·м)

%% ========== CVT карта ==========
cvt_data = readtable('CVT.xlsx', 'VariableNamingRule', 'preserve');
cvt.ratio_all = cvt_data{:,1};
cvt.torque_all = cvt_data{:,2};
cvt.eta_all = cvt_data{:,3};
cvt.ratio_vec = unique(cvt.ratio_all, 'stable');
cvt_torque_vec = unique(cvt.torque_all, 'stable');
cvt.eta_grid = reshape(cvt.eta_all, length(cvt_torque_vec), length(cvt.ratio_vec))';
cvt_eta_surf = griddedInterpolant({cvt.ratio_vec, cvt_torque_vec}, cvt.eta_grid, 'linear', 'none');
cvt.ratio_min = 0.427;
cvt.ratio_max = 2.561;

%% ========== Электромотор и батарея ==========
mg = struct();
mg.P_mg_max = 15;        % кВт
mg.rpm_tmax_curve = [0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500];
mg.torque_tmax_curve = [89, 88, 88, 88, 88, 88, 88, 80, 72, 65, 59, 54];
mg.T_max_func = @(w) interp1(mg.rpm_tmax_curve*(2*pi/60), mg.torque_tmax_curve, w, 'linear', 'extrap');
eta_inv = 0.85;
mg.E_bat_max = 0.8;      % кВт·ч

%% ========== Параметры стратегии ECMS ==========
hybrid = struct();
hybrid.s_mid      = 0.07;   % базовый эквивалентный фактор
hybrid.A_s        = 0.04;   % амплитуда сигмоида
hybrid.k_s        = 15;     % крутизна
hybrid.SOC_target = 0.60;   % целевой SOC (60%)
hybrid.SOC_init   = 0.60;

%% ========== Цикл WLTP ==========
data = readtable('WLTP_city.xlsx');
t = data.Time;
v = data.Speed / 3.6;        % м/с
dt = [0; diff(t)];

%% ========== Сетка передаточных чисел CVT ==========
i_cvt_grid = linspace(cvt.ratio_min, cvt.ratio_max, 54)';

%% ========== Инициализация ==========
N = length(t);
fuel_flow = zeros(N,1);          % г/с
rpm_points = zeros(N,1);         % обороты ДВС
torque_points = zeros(N,1);      % момент ДВС
mg_torque_points = zeros(N,1);   % момент МГ
mg_power_points = zeros(N,1);    % механическая мощность МГ (кВт)
mode_points = zeros(N,1);        % режим: 0-стоп, 1-рекуперация, 2-тяга
SOC_history = zeros(N,1);
SOC = hybrid.SOC_init;
SOC_history(1) = SOC;

%% ========== Главный цикл моделирования ==========
for k = 2:N
    vk = v(k);
    
    % Вычисляем текущий s(SOC)
    s_current = compute_s(SOC, hybrid);
    
    % ---------- ОСТАНОВКА ----------
    if vk == 0
        fuel_flow(k) = 0;
        mg_torque_points(k) = 0;
        mg_power_points(k) = 0;
        mode_points(k) = 0;
        rpm_points(k) = 0;
        torque_points(k) = 0;
        SOC_history(k) = SOC;
        continue;
    end
    
    % Ускорение и угловые скорости
    ak = (v(k) - v(k-1)) / dt(k);
    wk = vk / vehicle.r;
    ek = ak / vehicle.r;
    
    % Силы сопротивления
    F_roll = vehicle.m * vehicle.g * vehicle.Cr;
    F_aero = 0.5 * vehicle.rho * vehicle.Cd * vehicle.A * vk^2;
    F_iner = vehicle.m * ak;
    F_total = F_roll + F_aero + F_iner;
    
    % ---------- ТОРМОЖЕНИЕ / НАКАТ ----------
    if F_total <= 0
        T_wheel_brake = F_total * vehicle.r + vehicle.J_w * ek;  % отрицательный
        
        % Поиск оптимальной рекуперации (с учётом тормозного момента ДВС)
        best_J_brake = inf;
        best_T_mg = 0;
        best_P_batt = 0;
        best_P_mg_mech = 0;
        best_rpm_brake = 0;
        best_T_ice_brake = 0;
        
        for i_cvt = i_cvt_grid'
            w_eng = wk * vehicle.GP_ratio * i_cvt;
            rpm = w_eng * 60/(2*pi);
            if rpm < engine.rpm_min || rpm > engine.rpm_max, continue; end
            
            T_cvt_out = T_wheel_brake / (vehicle.GP_ratio * vehicle.Tsmn_eff) + vehicle.J_t * ek * vehicle.GP_ratio;
            CVT_eff_brake = CVT_map(i_cvt, abs(T_cvt_out));
            if isnan(CVT_eff_brake), continue; end
            T_in_req = T_cvt_out / (i_cvt * CVT_eff_brake) + vehicle.J_e * ek * vehicle.GP_ratio * i_cvt;  % отриц.
            
            % Тормозной момент ДВС (отсечка топлива)
            T_ice_brake = engine.T_brake(rpm);  % отрицательный
            T_mg_req = T_in_req - T_ice_brake;  % сколько нужно добавить МГ
            
            % Максимальный генераторный момент МГ
            T_mg_max_gen = -mg.T_max_func(w_eng);
            if T_mg_req < T_mg_max_gen
                T_mg_req = T_mg_max_gen;   % ограничиваем
            end
            if T_mg_req > 0
                continue;  % МГ не может тянуть при торможении
            end
            
            % Механическая мощность МГ
            P_mg_mech = T_mg_req * w_eng / 1000;   % кВт (отриц.)
            % Электрическая мощность на клеммах (зарядка)
            eta_mg = MG_efficiency(w_eng, T_mg_req, 'gen');
            P_batt = P_mg_mech * eta_mg * eta_inv;   % отриц.
            J_brake = s_current * P_batt;    % отрицательное (выгодно)
            
            if J_brake < best_J_brake
                best_J_brake = J_brake;
                best_T_mg = T_mg_req;
                best_P_batt = P_batt;
                best_P_mg_mech = P_mg_mech;
                best_rpm_brake = rpm;
                best_T_ice_brake = T_ice_brake;
            end
        end
        
        fuel_flow(k) = 0;   % отсечка топлива
        SOC = SOC - best_P_batt * dt(k) / 3600 / mg.E_bat_max;  % P_batt отриц., SOC растёт
        SOC = min(max(SOC, 0), 1);
        mg_torque_points(k) = best_T_mg;
        mg_power_points(k) = best_P_mg_mech;
        mode_points(k) = 1;
        rpm_points(k) = best_rpm_brake;
        torque_points(k) = best_T_ice_brake;
        SOC_history(k) = SOC;
        continue;
    end
    
    % ---------- ТЯГОВЫЙ РЕЖИМ (F_total > 0) ----------
    T_wheel = F_total * vehicle.r + vehicle.J_w * ek;
    
    best_J = inf;
    best_rpm = 0;
    best_T_eng = 0;
    best_T_mg = 0;
    best_fuel = 0;
    best_P_batt = 0;
    best_P_mg_mech = 0;
    
    for i_cvt = i_cvt_grid'
        w_eng = wk * vehicle.GP_ratio * i_cvt;
        rpm = w_eng * 60/(2*pi);
        if rpm < engine.rpm_min || rpm > engine.rpm_max, continue; end
        
        T_cvt_out = T_wheel / (vehicle.GP_ratio * vehicle.Tsmn_eff) + vehicle.J_t * ek * vehicle.GP_ratio;
        if T_cvt_out <= 0, continue; end
        CVT_eff = CVT_map(i_cvt, T_cvt_out);
        if isnan(CVT_eff), continue; end
        T_in_req = T_cvt_out / (i_cvt * CVT_eff) + vehicle.J_e * ek * vehicle.GP_ratio * i_cvt;
        
        % Максимальный момент ДВС
        T_ice_max_allowed = engine.Tmax(rpm);
        % Нижняя граница момента ДВС (не меньше T_ice_min, исключает EV)
        T_ice_low = max(engine.T_ice_min, T_in_req - mg.T_max_func(w_eng));
        % Верхняя граница момента ДВС
        T_ice_high = min(T_ice_max_allowed, T_in_req + mg.T_max_func(w_eng));
        if T_ice_low > T_ice_high, continue; end
        
        % Перебор момента ДВС
        for T_ice = T_ice_low:5:T_ice_high
            T_mg = T_in_req - T_ice;
            if abs(T_mg) > mg.T_max_func(w_eng) + 1e-6, continue; end
            
            % Расход топлива
            bsfc = BSFC(rpm, T_ice);
            P_eng = T_ice * w_eng / 1000;
            fuel = bsfc * P_eng / 3600;  % г/с
            
            % Механическая мощность МГ
            P_mg_mech = T_mg * w_eng / 1000;   % кВт
            if T_mg >= 0
                % Моторный режим (помощь)
                eta_mg = MG_efficiency(w_eng, T_mg, 'motor');
                P_batt = P_mg_mech / (eta_mg * eta_inv);
            else
                % Генераторный режим (заряд)
                eta_mg = MG_efficiency(w_eng, T_mg, 'gen');
                P_batt = P_mg_mech * eta_mg * eta_inv;
            end
            
            % Обобщённый расход
            J = fuel + s_current * P_batt;
            
            if J < best_J
                best_J = J;
                best_rpm = rpm;
                best_T_eng = T_ice;
                best_T_mg = T_mg;
                best_fuel = fuel;
                best_P_batt = P_batt;
                best_P_mg_mech = P_mg_mech;
            end
        end
    end
    
    % Применяем оптимальный режим
    fuel_flow(k) = best_fuel;
    mg_torque_points(k) = best_T_mg;
    mg_power_points(k) = best_P_mg_mech;
    mode_points(k) = 2;
    rpm_points(k) = best_rpm;
    torque_points(k) = best_T_eng;
    SOC = SOC - best_P_batt * dt(k) / 3600 / mg.E_bat_max;
    SOC = min(max(SOC, 0), 1);
    SOC_history(k) = SOC;
end

%% ========== Итоговые расчёты ==========
fuel_total_real = sum(fuel_flow .* dt);           % граммы
distance = trapz(t, v);                           % метры

delta_SOC = hybrid.SOC_init - SOC;
% Эквивалентный расход топлива за изменение SOC
fuel_eq_SOC = delta_SOC * mg.E_bat_max * 3600 * hybrid.s_mid;  % граммы
fuel_total_corrected = fuel_total_real + fuel_eq_SOC;

fuel_l_real = fuel_total_real / 745;              % литры
fuel_l_corr = fuel_total_corrected / 745;
result_real = fuel_l_real / (distance / 100000);  % л/100 км
result_corr = fuel_l_corr / (distance / 100000);

fprintf('Реальный расход: %.2f л/100км\n', result_real);
fprintf('Скорректированный расход (с учётом SOC): %.2f л/100км\n', result_corr);
fprintf('Конечный SOC: %.1f %%\n', SOC*100);

%% ========== Графики ==========
% 1. Мгновенный расход топлива и мощность МГ
figure;
yyaxis left;
plot(t, fuel_flow, 'b-', 'LineWidth', 1.2);
ylabel('Расход топлива, г/с');
yyaxis right;
plot(t, mg_power_points, 'r-', 'LineWidth', 1.2);
ylabel('Мощность МГ, кВт (полож. - тяга, отриц. - генерация)');
xlabel('Время, с');
title('Мгновенный расход топлива и мощность электромотора');
grid on;
legend('Расход топлива', 'Мощность МГ', 'Location','best');

% 2. SOC
figure;
plot(t, SOC_history*100, 'k-', 'LineWidth', 1.5);
xlabel('Время, с'); ylabel('SOC, %');
title('Уровень заряда батареи'); grid on;

% 3. Профиль скорости
figure;
plot(t, v*3.6, 'b-', 'LineWidth', 1.5);
xlabel('Время, с'); ylabel('Скорость, км/ч');
title('Профиль скорости WLTP'); grid on;

% 4. Рабочие точки ДВС (только тяговый режим)
figure;
idx_ice = (mode_points == 2);   % только гибридная тяга
scatter(rpm_points(idx_ice), torque_points(idx_ice), 15, 'filled');
xlabel('Обороты ДВС, об/мин'); ylabel('Крутящий момент ДВС, Н·м');
title('Рабочие точки ДВС (тяговый режим)');
xlim([0 6000]); ylim([0 150]);
grid on;

%% Значение топливного эквивалента

function s = compute_s(SOC, hybrid)
    % Адаптивный эквивалентный фактор s(SOC)
    % SOC – уровень заряда (0..1)
    % hybrid – структура с параметрами s_mid, A_s, k_s, SOC_target
    s = hybrid.s_mid - hybrid.A_s * tanh(hybrid.k_s * (SOC - hybrid.SOC_target));
end

%% Значение потерь в электродвигателе

function eta = MG_efficiency(w, T, mode)
    % Вычисление КПД электромотора-генератора (PMSM) по аналитической модели потерь
    % w   - угловая скорость (рад/с)
    % T   - электромагнитный момент (Н·м), положительный для motor, отрицательный для gen
    % mode - 'motor' или 'gen'
    
    % Параметры потерь (подбираются под конкретный 15 кВт мотор)
    k_c = 0.005;      % коэффициент потерь в меди, 1/(Н·м)² * ? – калибруется
    k_i = 2.0;        % коэффициент потерь в стали + мех., Вт/(рад/с)
    k_w = 0.1;        % коэффициент вентиляционных потерь, Вт/(рад/с)^1.5
    C   = 50;         % постоянные потери (контроллер и пр.), Вт
    
    if abs(T) < 0.01 || w < 1.0
        eta = 0.5;    % при нулевой нагрузке КПД низкий, чтобы избежать деления на ноль
        return;
    end
    
    P_mech = abs(T) * w;   % механическая мощность, Вт
    
    % Потери
    P_loss = k_c * T^2 + k_i * w + k_w * w^1.5 + C;
    
    if strcmpi(mode, 'motor')
        % Потребляемая электрическая мощность больше механической
        eta = P_mech / (P_mech + P_loss);
    else
        % Генераторный режим: отдаваемая электрическая мощность меньше механической
        % P_mg = P_mech - P_loss
        eta = (P_mech - P_loss) / P_mech;
    end
    
    % Ограничение разумными пределами
    eta = min(max(eta, 0.5), 0.97);
end

%% Значение потерь в вариаторе

function eta = CVT_map(i_val, T_out_val)
    
    global cvt_eta_surf cvt_torque_vec %#ok<GVMIS>
    
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