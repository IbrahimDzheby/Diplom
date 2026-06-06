clc; clear; close all;

% --- Единый шрифт ГОСТ Type A (наклонный) ---
set(0, 'DefaultTextFontSize', 16);
set(0, 'DefaultTextFontName', 'GOST Type A');
set(0, 'DefaultTextFontAngle', 'italic');
set(0, 'DefaultAxesFontSize', 16);
set(0, 'DefaultAxesFontName', 'GOST Type A');
set(0, 'DefaultAxesFontAngle', 'italic');

%% Исходные данные

global k_aux k_mt cvt_torque_vec cvt_eta_surf %#ok<GVMIS>

mass = 1600; % масса автомобиля, кг
wheel_radius = 0.285; % м 175/65 R15
AB = 0.45; % развесоква для передней оси

mu = 0.85; % коэффициент сцепления
k_aux = 0.95; % коэффициент потерь на вспомогательные системы
k_mt = 0.95;  % коэффициент потерь главной передаче

final_drive = 5.4;

%% ========== CVT карта ==========

cvt_data = readtable('CVT.xlsx', 'VariableNamingRule', 'preserve');
cvt_ratio_all = cvt_data{:,1};
cvt_torque_all = cvt_data{:,2};
cvt_eta_all = cvt_data{:,3};
cvt_ratio_vec = unique(cvt_ratio_all, 'stable');
cvt_torque_vec = unique(cvt_torque_all, 'stable');
cvt_eta_grid = reshape(cvt_eta_all, length(cvt_torque_vec), length(cvt_ratio_vec))';
cvt_eta_surf = griddedInterpolant({cvt_ratio_vec, cvt_torque_vec}, cvt_eta_grid, 'linear', 'none');

i_CVT_max = 2.561; % минимальная передача (низшая)
i_CVT_min = 0.427; % максимальная передача (высшая)

function eta = CVT_map(i_val, T_in_val)
    global k_aux k_mt cvt_eta_surf %#ok<GVMIS>
    eta = k_aux * k_mt * cvt_eta_surf(i_val, T_in_val);
end

%% Входные данные ДВС
n_points = 13;
 
RPM = [800, 1113.39, 1614.81, 2015.95, 2404.56, 3006.27, 3231.91, 3607.98, 4009.12, 4372.65, 4686.04, 4986.89, 5200];
Torque = [101.10, 106.59, 116.48, 123.08, 131.32, 136.81, 138.46, 141.76, 142.86, 140.11, 137.91, 136.26, 134.07]; % крутящий момент Нм
Power = (Torque .* RPM * 2 * pi / 60) / 1000; % мощность в кВт

%% Минимальные и максимальные обороты

RPM_min = RPM(1);
RPM_max = RPM(end);

Torque_min_RPM = Torque(1);         % момент при min оборотах
Torque_max = max(Torque);           % max момент
Torque_max_RPM = Torque(end);       % момент при max оборотах

%% Внешняя скоростная характеристика

figure;
xlabel('Обороты двигателя, об/мин');
title('Внешняя скоростная характеристика ДВС','FontWeight', 'normal');
grid on

% --- Левая ось Y (Момент) ---
yyaxis left
plot(RPM, Torque, '-b', 'LineWidth', 3)
ylabel('Момент, Нм')
ylim([(min(Torque)-5) (max(Torque)+5)]);

% --- Правая ось Y (Мощность) ---
yyaxis right
plot(RPM, Power, '-r', 'LineWidth', 3)
ylabel('Мощность, кВт')
ylim([0 (max(Power)+5)]);

% Чёрные шкалы для обеих осей и оси X
ax = gca;
ax.YAxis(1).Color = 'k';   % левая ось Y
ax.YAxis(2).Color = 'k';   % правая ось Y
ax.XAxis.Color = 'k';      % ось X

legend('Момент', 'Мощность', 'Location', 'southeast')

%% Характеристика момента

figure;

plot(RPM, Torque, '-b', 'LineWidth', 3);
xlabel('Обороты двигателя, об/мин');
ylabel('Момент, Нм');
title('Моментная характеристика ДВС','FontWeight', 'normal');
grid on;

set(gca);
ylim([(min(Torque)-5) (max(Torque)+5)]);

%% Кривая сопротивлений движения автомобиля

% Скорость для сопротивлений (0..200 км/ч)
v_res_kmh = linspace(0, 250, n_points);
v_res = v_res_kmh / 3.6; % перевод в м/с

% Параметры сопротивлений
Cr = 0.012;      % коэффициент качения
Cd = 0.31;       % аэродинамика
A = 2.2;         % площадь, м^2
rho = 1.225;     % плотность воздуха
g = 9.81;

% Силы сопротивлений
F_roll = Cr * mass * g;
F_aero = 0.5 * rho * Cd * A .* v_res.^2;
F_res = F_roll + F_aero;

%% Расчет тяговой характеристики

i_range = linspace(i_CVT_min, i_CVT_max, n_points);

% Кривая тяги от скорости на высшей передаче
% eta_low  = CVT_map(i_CVT_max, Torque); % массив коэффициентов эффективности
eta_low  = arrayfun(@(t) CVT_map(i_CVT_max, t), Torque);
v_low  = (2*pi*RPM/60) * wheel_radius / (i_CVT_max*final_drive) * 3.6;
F_low  = Torque .* eta_low .* i_CVT_max * final_drive / wheel_radius;

% Кривая тяги от скорости на низшей передаче
% eta_high = CVT_map(i_CVT_min, Torque); % массив коэффициентов эффективности
eta_high = arrayfun(@(t) CVT_map(i_CVT_min, t), Torque);
v_high = (2*pi*RPM/60) * wheel_radius / (i_CVT_min*final_drive) * 3.6;
F_high = Torque .* eta_high .* i_CVT_min * final_drive / wheel_radius;

% Изменение тяги на низших оборотах от передаточного числа
% eta_left = CVT_map(i_range, Torque_min_RPM); % массив коэффициентов эффективности
eta_left = arrayfun(@(t) CVT_map(t, Torque_min_RPM), i_range);
v_left  = (2*pi*RPM_min/60) * wheel_radius ./ (i_range*final_drive) * 3.6;
F_left  = Torque_min_RPM .* eta_left .* i_range * final_drive / wheel_radius;

% Изменение тяги на высших оборотах от передаточного числа
% eta_right = CVT_map(i_range, Torque_max_RPM); % массив коэффициентов эффективности
eta_right = arrayfun(@(t) CVT_map(t, Torque_max_RPM), i_range);
v_right = (2*pi*RPM_max/60) * wheel_radius ./ (i_range*final_drive) * 3.6;
F_right = Torque_max_RPM .* eta_right .* i_range * final_drive / wheel_radius;

%% Построение тяговой характеристики

figure;
grid on;
xlabel('Скорость, км/ч')
ylabel('Тяга на колёсах, Н')
title('Тяговая характеристика ДВС','FontWeight', 'normal');

xlim([0 250]); % График до 200 км/ч

hold on;

% Левая граница (min RPM)
plot(v_left,  F_left,  'blue--', 'LineWidth', 2);

% Правая граница (max RPM)
plot(v_right, F_right, 'blue--', 'LineWidth', 2);

% Верхняя (низшая передача)
plot(v_low,  F_low, '-b', 'LineWidth', 3);

% Нижняя (высшая передача)
plot(v_high, F_high, '-b', 'LineWidth', 3);

% Сопротивление движению
plot(v_res_kmh, F_res, '-r', 'LineWidth', 2);

hold off;

legend('','min и max обороты','','min и max передача', ...
    'Сопротивление движению');

%% Расчет динамической характеристики

% Свободная тяга
F_free_low  = F_low - (0.5 * rho * Cd * A .* (v_low/3.6).^2);
F_free_high = F_high - (0.5 * rho * Cd * A .* (v_high/3.6).^2);
F_free_left  = F_left - (0.5 * rho * Cd * A .* (v_left/3.6).^2);
F_free_right = F_right - (0.5 * rho * Cd * A .* (v_right/3.6).^2);

% Динамический фаткор
DF_low  = F_free_low  ./ (mass * g);
DF_high = F_free_high ./ (mass * g);
DF_left  = F_free_left  ./ (mass * g);
DF_right = F_free_right ./ (mass * g);

%% Построение динамического фактора

figure;
grid on;
xlabel('Скорость, км/ч')
ylabel('Динамический фактор')
title('Динамическая характеристика ДВС','FontWeight', 'normal');

xlim([0 250]); % График до 200 км/ч

hold on;

% Левая граница (min RPM)
plot(v_left,  DF_left,  'blue--', 'LineWidth', 2);

% Правая граница (max RPM)
plot(v_right, DF_right, 'blue--', 'LineWidth', 2);

% Верхняя (низшая передача)
plot(v_low,   DF_low, '-b', 'LineWidth', 3);

% Нижняя (высшая передача)
plot(v_high,  DF_high, '-b', 'LineWidth', 3);

D_16 = 0.1 + Cr;

plot([0 250], [D_16 D_16], '--r', 'LineWidth', 2);

hold off;

legend('','min и max обороты','','min и max передача', 'Подъем 10%');

%% Расчет ускорения автомобиля

% --- Полные сопротивления движения ---
F_res_low   = Cr * mass * g + 0.5 * rho * Cd * A .* (v_low / 3.6).^2;
F_res_high  = Cr * mass * g + 0.5 * rho * Cd * A .* (v_high / 3.6).^2;
F_res_left  = Cr * mass * g + 0.5 * rho * Cd * A .* (v_left / 3.6).^2;
F_res_right = Cr * mass * g + 0.5 * rho * Cd * A .* (v_right / 3.6).^2;

% --- Ускорение: a = (Fт - Fсопр) / m ---
a_low   = (F_low   - F_res_low)   ./ mass;
a_high  = (F_high  - F_res_high)  ./ mass;
a_left  = (F_left  - F_res_left)  ./ mass;
a_right = (F_right - F_res_right) ./ mass;

%% Построение графика ускорения

figure;
grid on;
xlabel('Скорость, км/ч')
ylabel('Ускорение, м/с^2')
title('Характеристика ускорения ДВС','FontWeight', 'normal');
xlim([0 250])

hold on;

% Левая граница (min RPM)
plot(v_left,  a_left,  'blue--', 'LineWidth', 2);

% Правая граница (max RPM)
plot(v_right, a_right, 'blue--', 'LineWidth', 2);

% Низшая передача
plot(v_low,  a_low,  '-b', 'LineWidth', 3);

% Высшая передача
plot(v_high, a_high, '-b', 'LineWidth', 3);

hold off;

legend('','min и max обороты','','min и max передача');