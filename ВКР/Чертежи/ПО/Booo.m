clc; clear; close all;

%% ========== Параметры автомобиля ==========
vehicle = struct();
vehicle.m     = 1600;
vehicle.r     = 0.285;
vehicle.Cr    = 0.012;
vehicle.Cd    = 0.31;
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
engine.rpm_tmax_curve = [800, 1113.39, 1614.81, 2015.95, 2404.56, ...
    3006.27, 3231.91, 3607.98, 4009.12, 4372.65, 4686.04, 4986.89, 5200];
engine.torque_tmax_curve = [101.10, 106.59, 116.48, 123.08, 131.32, ...
    136.81, 138.46, 141.76, 142.86, 140.11, 137.91, 136.26, 134.07];
engine.Tmax = @(rpm) interp1(engine.rpm_tmax_curve, engine.torque_tmax_curve, rpm, 'linear', 'extrap');
engine.T_ice_min = 10;
engine.T_brake = @(rpm) max(-50, -0.02 * rpm);

%% ========== CVT карта ==========
global cvt_eta_surf cvt_torque_vec
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
mg.P_mg_max = 15;
mg.rpm_tmax_curve = [0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500];
mg.torque_tmax_curve = [89, 88, 88, 88, 88, 88, 88, 80, 72, 65, 59, 54];
mg.T_max_func = @(w) interp1(mg.rpm_tmax_curve*(2*pi/60), mg.torque_tmax_curve, w, 'linear', 'extrap');
eta_inv = 0.85;
mg.E_bat_max = 0.8;      % кВт·ч

%% ========== Параметры стратегии ECMS ==========
SOC_points = [0, 0.20, 0.60, 1];
s_points   = [0.193, 0.193, 0.077, 0.059];
hybrid = struct();
hybrid.SOC_init = 0.60;
hybrid.s_mid = 0.077;

%% ========== Цикл WLTP ==========
data = readtable('WLTP.xlsx');
t = data.Time;
v = data.Speed / 3.6;
dt = [0; diff(t)];

%% ========== Сетка передаточных чисел CVT ==========
i_cvt_grid = linspace(cvt.ratio_min, cvt.ratio_max, 54)';

%% ========== Инициализация ==========
N = length(t);
fuel_flow = zeros(N,1);
rpm_points = zeros(N,1);
torque_points = zeros(N,1);
mg_torque_points = zeros(N,1);
mg_power_points = zeros(N,1);
mode_points = zeros(N,1);
SOC_history = zeros(N,1);
SOC = hybrid.SOC_init;
SOC_history(1) = SOC;

% Новые массивы:
i_cvt_history = NaN(N,1);   % передаточное число вариатора
pedals = zeros(N,1);        % педаль газа (0..1)

%% ========== Главный цикл моделирования ==========
for k = 2:N
    vk = v(k);
    s_current = compute_s(SOC);
    
    % ---------- ОСТАНОВКА ----------
    if vk == 0
        fuel_flow(k) = 0;
        mg_torque_points(k) = 0;
        mg_power_points(k) = 0;
        mode_points(k) = 0;
        rpm_points(k) = 0;
        torque_points(k) = 0;
        SOC_history(k) = SOC;
        i_cvt_history(k) = NaN;
        pedals(k) = 0;
        continue;
    end
    
    ak = (v(k) - v(k-1)) / dt(k);
    wk = vk / vehicle.r;
    ek = ak / vehicle.r;
    
    F_roll = vehicle.m * vehicle.g * vehicle.Cr;
    F_aero = 0.5 * vehicle.rho * vehicle.Cd * vehicle.A * vk^2;
    F_iner = vehicle.m * ak;
    F_total = F_roll + F_aero + F_iner;
    
    % ---------- ТОРМОЖЕНИЕ / НАКАТ ----------
    if F_total <= 0
        T_wheel_brake = F_total * vehicle.r + vehicle.J_w * ek;
        best_J_brake = inf;
        best_T_mg = 0;
        best_P_batt = 0;
        best_P_mg_mech = 0;
        best_rpm_brake = 0;
        best_T_ice_brake = 0;
        best_i_cvt = NaN;
        
        for i_cvt = i_cvt_grid'
            w_eng = wk * vehicle.GP_ratio * i_cvt;
            rpm = w_eng * 60/(2*pi);
            if rpm < engine.rpm_min || rpm > engine.rpm_max, continue; end
            
            T_cvt_out = T_wheel_brake / (vehicle.GP_ratio * vehicle.Tsmn_eff) + ...
                vehicle.J_t * ek * vehicle.GP_ratio;
            CVT_eff_brake = CVT_map(i_cvt, abs(T_cvt_out));
            if isnan(CVT_eff_brake), continue; end
            T_in_req = T_cvt_out / (i_cvt * CVT_eff_brake) + ...
                vehicle.J_e * ek * vehicle.GP_ratio * i_cvt;
            
            T_ice_brake = engine.T_brake(rpm);
            T_mg_req = T_in_req - T_ice_brake;
            T_mg_max_gen = -mg.T_max_func(w_eng);
            if T_mg_req < T_mg_max_gen
                T_mg_req = T_mg_max_gen;
            end
            if T_mg_req > 0, continue; end
            
            P_mg_mech = T_mg_req * w_eng / 1000;
            eta_mg = MG_efficiency(w_eng, T_mg_req, 'gen');
            P_batt = P_mg_mech * eta_mg * eta_inv;
            J_brake = s_current * P_batt;
            
            if J_brake < best_J_brake
                best_J_brake = J_brake;
                best_T_mg = T_mg_req;
                best_P_batt = P_batt;
                best_P_mg_mech = P_mg_mech;
                best_rpm_brake = rpm;
                best_T_ice_brake = T_ice_brake;
                best_i_cvt = i_cvt;
            end
        end
        
        fuel_flow(k) = 0;
        SOC = SOC - best_P_batt * dt(k) / 3600 / mg.E_bat_max;
        SOC = min(max(SOC, 0), 1);
        mg_torque_points(k) = best_T_mg;
        mg_power_points(k) = best_P_mg_mech;
        mode_points(k) = 1;
        rpm_points(k) = best_rpm_brake;
        torque_points(k) = best_T_ice_brake;
        SOC_history(k) = SOC;
        i_cvt_history(k) = best_i_cvt;
        pedals(k) = 0;
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
    best_i_cvt = NaN;
    
    for i_cvt = i_cvt_grid'
        w_eng = wk * vehicle.GP_ratio * i_cvt;
        rpm = w_eng * 60/(2*pi);
        if rpm < engine.rpm_min || rpm > engine.rpm_max, continue; end
        
        T_cvt_out = T_wheel / (vehicle.GP_ratio * vehicle.Tsmn_eff) + ...
            vehicle.J_t * ek * vehicle.GP_ratio;
        if T_cvt_out <= 0, continue; end
        CVT_eff = CVT_map(i_cvt, T_cvt_out);
        if isnan(CVT_eff), continue; end
        T_in_req = T_cvt_out / (i_cvt * CVT_eff) + ...
            vehicle.J_e * ek * vehicle.GP_ratio * i_cvt;
        
        T_ice_max_allowed = engine.Tmax(rpm);
        T_ice_low = max(engine.T_ice_min, T_in_req - mg.T_max_func(w_eng));
        T_ice_high = min(T_ice_max_allowed, T_in_req + mg.T_max_func(w_eng));
        if T_ice_low > T_ice_high, continue; end
        
        for T_ice = T_ice_low:5:T_ice_high
            T_mg = T_in_req - T_ice;
            if abs(T_mg) > mg.T_max_func(w_eng) + 1e-6, continue; end
            
            bsfc = BSFC(rpm, T_ice);
            P_eng = T_ice * w_eng / 1000;
            fuel = bsfc * P_eng / 3600;
            
            P_mg_mech = T_mg * w_eng / 1000;
            if T_mg >= 0
                eta_mg = MG_efficiency(w_eng, T_mg, 'motor');
                P_batt = P_mg_mech / (eta_mg * eta_inv);
            else
                eta_mg = MG_efficiency(w_eng, T_mg, 'gen');
                P_batt = P_mg_mech * eta_mg * eta_inv;
            end
            J = fuel + s_current * P_batt;
            
            if J < best_J
                best_J = J;
                best_rpm = rpm;
                best_T_eng = T_ice;
                best_T_mg = T_mg;
                best_fuel = fuel;
                best_P_batt = P_batt;
                best_P_mg_mech = P_mg_mech;
                best_i_cvt = i_cvt;
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
    i_cvt_history(k) = best_i_cvt;
    
    % Педаль газа: отношение текущей силы тяги к максимальной (6000 Н)
    F_traction = T_wheel / vehicle.r;
    pedals(k) = min(1, max(0, F_traction / 6000));
end

%% ========== Итоговые расчёты (только для справки, без графиков) ==========
fuel_total_real = sum(fuel_flow .* dt);
distance = trapz(t, v);
delta_SOC = hybrid.SOC_init - SOC;
fuel_eq_SOC = delta_SOC * mg.E_bat_max * 3600 * hybrid.s_mid;
fuel_total_corrected = fuel_total_real + fuel_eq_SOC;
fuel_l_real = fuel_total_real / 745;
fuel_l_corr = fuel_total_corrected / 745;
result_real = fuel_l_real / (distance / 100000);
result_corr = fuel_l_corr / (distance / 100000);
fprintf('Реальный расход: %.2f л/100км\n', result_real);
fprintf('Скорректированный расход: %.2f л/100км\n', result_corr);
fprintf('Конечный SOC: %.1f %%\n', SOC*100);

%% ========== Постобработка: заполнение пропусков в передаточном числе ==========
% Заменяем NaN на предыдущее допустимое значение
i_cvt_history = fillmissing(i_cvt_history, 'previous');

% 1. Скорость
figure('Name', 'Скорость автомобиля', 'NumberTitle', 'off');
plot(t, v*3.6, 'b-', 'LineWidth', 2);
xlabel('Время, с');
ylabel('Скорость, км/ч');
grid on;

% 2. Передаточное число вариатора
figure('Name', 'Передаточное число CVT', 'NumberTitle', 'off');
plot(t, i_cvt_history, '-','Color', [1 0.5 0], 'LineWidth', 2);
xlabel('Время, с');
ylabel('i_{CVT}');
grid on; ylim([cvt.ratio_min cvt.ratio_max]);

% 3. Момент ДВС
figure('Name', 'Момент ДВС', 'NumberTitle', 'off');
plot(t, torque_points, 'r-', 'LineWidth', 2);
xlabel('Время, с');
ylabel('Крутящий момент ДВС, Н·м');
grid on;

% 4. Момент МГ
figure('Name', 'Момент электромотора', 'NumberTitle', 'off');
plot(t, mg_torque_points, 'm-', 'LineWidth', 2);
xlabel('Время, с');
ylabel('Момент МГ, Н·м');
grid on;
% 5. SOC
figure('Name', 'Состояние заряда батареи', 'NumberTitle', 'off');
plot(t, SOC_history*100, 'k-', 'LineWidth', 2);
xlabel('Время, с');
ylabel('SOC, %');
grid on;
ylim([0 100]);

% 6. Положение педали газа
figure('Name', 'Педаль газа', 'NumberTitle', 'off');
plot(t, pedals*100, 'k-', 'LineWidth', 2);
xlabel('Время, с');
ylabel('Педаль газа, %');
grid on;
ylim([0 100]);

%% ========== Вспомогательные функции (остаются без изменений) ==========
function s = compute_s(SOC)
    SOC_pts = [0, 0.3, 0.5, 0.6, 1];
    s_pts   = [0.11, 0.1, 0.07, 0.04, 0.03];
    s = interp1(SOC_pts, s_pts, SOC, 'linear', 'extrap');
    s = min(max(s, 0.02), 0.15);
end

function eta = MG_efficiency(w, T, mode)
    k_c = 0.005; k_i = 2.0; k_w = 0.1; C = 50;
    if abs(T) < 0.01 || w < 1.0
        eta = 0.5; return;
    end
    P_mech = abs(T) * w;
    P_loss = k_c * T^2 + k_i * w + k_w * w^1.5 + C;
    if strcmpi(mode, 'motor')
        eta = P_mech / (P_mech + P_loss);
    else
        eta = (P_mech - P_loss) / P_mech;
    end
    eta = min(max(eta, 0.5), 0.97);
end

function eta = CVT_map(i_val, T_out_val)
    global cvt_eta_surf cvt_torque_vec
    cost = @(T_in) i_val * T_in * cvt_eta_surf(i_val, T_in) - T_out_val;
    options = optimset('Display','off');
    try
        T_sol = fzero(cost, [min(cvt_torque_vec), max(cvt_torque_vec)], options);
        eta = cvt_eta_surf(i_val, T_sol);
    catch
        eta = NaN;
    end
end

function bsfc = BSFC(rpm, torque)
    persistent vertices triangles rpm_min rpm_max torque_min torque_max nTri
    if isempty(vertices)
        S = coder.load('BSFC_surface.mat');
        vertices = S.all_vertices;
        triangles = S.all_triangles;
        rpm_min = min(vertices(:,1)); rpm_max = max(vertices(:,1));
        torque_min = min(vertices(:,2)); torque_max = max(vertices(:,2));
        nTri = size(triangles, 1);
    end
    if rpm < rpm_min || rpm > rpm_max || torque < torque_min || torque > torque_max
        bsfc = NaN; return;
    end
    for t = 1:nTri
        i1 = triangles(t, 1); i2 = triangles(t, 2); i3 = triangles(t, 3);
        p1 = vertices(i1, :); p2 = vertices(i2, :); p3 = vertices(i3, :);
        if pointInTriangle([rpm, torque], p1(1:2), p2(1:2), p3(1:2))
            bsfc = barycentricInterp(p1, p2, p3, [rpm, torque]);
            return;
        end
    end
    bsfc = NaN;
end

function inside = pointInTriangle(pt, p1, p2, p3)
    v0 = p3 - p1; v1 = p2 - p1; v2 = pt - p1;
    dot00 = v0(1)*v0(1) + v0(2)*v0(2);
    dot01 = v0(1)*v1(1) + v0(2)*v1(2);
    dot02 = v0(1)*v2(1) + v0(2)*v2(2);
    dot11 = v1(1)*v1(1) + v1(2)*v1(2);
    dot12 = v1(1)*v2(1) + v1(2)*v2(2);
    invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    inside = (u >= 0) && (v >= 0) && (u + v <= 1);
end

function z = barycentricInterp(p1, p2, p3, pt)
    % барицентрическая интерполяция
    x = pt(1); y = pt(2);
    x1 = p1(1); y1 = p1(2); z1 = p1(3);
    x2 = p2(1); y2 = p2(2); z2 = p2(3);
    x3 = p3(1); y3 = p3(2); z3 = p3(3);
    denom = (y2-y3)*(x1-x3)+(x3-x2)*(y1-y3);
    if denom == 0
        z = z1;
    else
        a = ((y2-y3)*(x-x3)+(x3-x2)*(y-y3)) / denom;
        b = ((y3-y1)*(x-x3)+(x1-x3)*(y-y3)) / denom;
        c = 1 - a - b;
        z = a*z1 + b*z2 + c*z3;
    end
end