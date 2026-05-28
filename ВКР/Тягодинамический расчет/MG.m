clc; clear; close all;

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

%% Входные данные МГ
n_points = 12;
RPM = [0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5200];
TorquePeak = [88, 88, 88, 88, 88, 88, 88, 80, 72, 65, 59, 57];
Torque = [41, 42, 43, 43.75, 44.5, 45, 45.5, 45.75, 42.5, 38, 34.5, 33.5];
PowerPeak = (TorquePeak .* RPM * 2 * pi / 60) / 1000;
Power = (Torque .* RPM * 2 * pi / 60) / 1000;

%% Моментная характеристика EV

figure;
grid on;
xlabel('Обороты двигателя, об/мин');
ylabel('Момент, Нм');

hold on;

plot(RPM, Torque, '-b', 'LineWidth', 2);
plot(RPM, TorquePeak, '--', 'Color', [1 0.5 0], 'LineWidth', 2);

legend('Номинальный момент', 'Пиковый момент');
hold off;

% set(gca);
% ylim([(min(Torque)-5) (max(Torque)+5)]);

%% Мощность EV

figure;
grid on;
xlabel('Обороты двигателя, об/мин');
ylabel('Мощность, кВт');

hold on;

plot(RPM, Power, '-b', 'LineWidth', 2);
plot(RPM, PowerPeak, '--', 'Color', [1 0.5 0], 'LineWidth', 2);

legend('Номинальная мощность', 'Пиковая мощность');
hold off;

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

eta_low_peak  = arrayfun(@(t) CVT_map(i_CVT_max, t), TorquePeak);
eta_low  = arrayfun(@(t) CVT_map(i_CVT_max, t), Torque);

v_low  = (2*pi*RPM/60) * wheel_radius / (i_CVT_max*final_drive) * 3.6;
F_low_peak  = TorquePeak .* eta_low .* i_CVT_max * final_drive / wheel_radius;
F_low  = Torque .* eta_low .* i_CVT_max * final_drive / wheel_radius;

% Кривая тяги от скорости на низшей передаче

eta_high_peak  = arrayfun(@(t) CVT_map(i_CVT_min, t), TorquePeak);
eta_high = arrayfun(@(t) CVT_map(i_CVT_min, t), Torque);

v_high = (2*pi*RPM/60) * wheel_radius / (i_CVT_min*final_drive) * 3.6;
F_high_peak = TorquePeak .* eta_high .* i_CVT_min * final_drive / wheel_radius;
F_high = Torque .* eta_high .* i_CVT_min * final_drive / wheel_radius;

% Изменение тяги на высших оборотах от передаточного числа

eta_right_peak  = arrayfun(@(t) CVT_map(t, Torque(end)), i_range);
eta_right = arrayfun(@(t) CVT_map(t, Torque(end)), i_range);

v_right = (2*pi*RPM(end)/60) * wheel_radius ./ (i_range*final_drive) * 3.6;
F_right_peak = TorquePeak(end) .* eta_right .* i_range * final_drive / wheel_radius;
F_right = Torque(end) .* eta_right .* i_range * final_drive / wheel_radius;

%% Построение тяговой характеристики

figure;
grid on;
xlabel('Скорость, км/ч')
ylabel('Тяга на колёсах, Н')

xlim([0 250]); % График до 200 км/ч

hold on;

% Правая граница (max RPM)
plot(v_right, F_right, 'blue--', 'LineWidth', 1);

% Верхняя (низшая передача)
plot(v_low,  F_low, '-b', 'LineWidth', 2);

% Нижняя (высшая передача)
plot(v_high, F_high, '-b', 'LineWidth', 2);

% Сопротивление движению
plot(v_res_kmh, F_res, '-r', 'LineWidth', 2);

hold off;

legend('max обороты','','min и max передача', ...
    'Сопротивление движению');

%% Расчет динамической характеристики

% Свободная тяга
F_free_low_peak  = F_low_peak - (0.5 * rho * Cd * A .* (v_low/3.6).^2);
F_free_low  = F_low - (0.5 * rho * Cd * A .* (v_low/3.6).^2);

F_free_high_peak = F_high_peak - (0.5 * rho * Cd * A .* (v_high/3.6).^2);
F_free_high = F_high - (0.5 * rho * Cd * A .* (v_high/3.6).^2);

F_free_right_peak = F_right_peak - (0.5 * rho * Cd * A .* (v_right/3.6).^2);
F_free_right = F_right - (0.5 * rho * Cd * A .* (v_right/3.6).^2);

% Динамический фаткор
DF_low_peak  = F_free_low_peak  ./ (mass * g);
DF_low  = F_free_low  ./ (mass * g);

DF_high_peak = F_free_high_peak ./ (mass * g);
DF_high = F_free_high ./ (mass * g);

DF_right_peak = F_free_right_peak ./ (mass * g);
DF_right = F_free_right ./ (mass * g);

%% Построение динамического фактора

figure;
grid on;
xlabel('Скорость, км/ч')
ylabel('Динамический фактор')

xlim([0 250]); % График до 200 км/ч

hold on;

% Правая граница (max RPM)
plot(v_right, DF_right_peak, '--', 'Color', [1 0.5 0], 'LineWidth', 1);
plot(v_right, DF_right, 'blue--', 'LineWidth', 1);

% Верхняя (низшая передача)
plot(v_low,   DF_low_peak, '-', 'Color', [1 0.5 0], 'LineWidth', 2);
plot(v_low,   DF_low, '-b', 'LineWidth', 2);

% Нижняя (высшая передача)
plot(v_high,  DF_high_peak, '-', 'Color', [1 0.5 0], 'LineWidth', 2);
plot(v_high,  DF_high, '-b', 'LineWidth', 2);

D_16 = 0.1 + Cr;

plot([0 250], [D_16 D_16], '--r', 'LineWidth', 1.5);

hold off;

legend('max обороты пик Момент','max обороты ном Момент','min и max передача пик Момент','min и max передача ном Момент', '', '', 'Подъем 10%');

%% Расчет ускорения автомобиля

% --- Полные сопротивления движения ---
F_res_low   = Cr * mass * g + 0.5 * rho * Cd * A .* (v_low / 3.6).^2;
F_res_high  = Cr * mass * g + 0.5 * rho * Cd * A .* (v_high / 3.6).^2;
F_res_right = Cr * mass * g + 0.5 * rho * Cd * A .* (v_right / 3.6).^2;

% --- Ускорение: a = (Fт - Fсопр) / m ---
a_low   = (F_low   - F_res_low)   ./ mass;
a_high  = (F_high  - F_res_high)  ./ mass;
a_right = (F_right - F_res_right) ./ mass;

%% Построение графика ускорения

figure;
grid on;
xlabel('Скорость, км/ч')
ylabel('Ускорение, м/с^2')
xlim([0 250])

hold on;

% Правая граница (max RPM)
plot(v_right, a_right, 'blue--', 'LineWidth', 1);

% Низшая передача
plot(v_low,  a_low,  '-b', 'LineWidth', 2);

% Высшая передача
plot(v_high, a_high, '-b', 'LineWidth', 2);

hold off;

legend('max обороты','','min и max передача');