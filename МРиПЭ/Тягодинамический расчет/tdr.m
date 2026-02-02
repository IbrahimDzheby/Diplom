clc;
clear;
close all;

%% Цвета для графиков

red = [0.6 0.1 0.1];
green = [0.0 0.45 0.0];
blue = [0.1 0.3 0.6];

%% Исходные данные
mass = 1600; % масса автомобиля, кг
wheel_radius = 0.285; % м 175/65 R15
AB = 0.45; % развесоква для передней оси

mu = 0.85; % коэффициент сцепления
k_aux = 0.95; % коэффициент потерь на вспомогательные системы
k_mt = 0.97;  % коэффициент потерь главной передачи

i_CVT_max = 2.561; % минимальная передача (низшая)
i_CVT_min = 0.427; % максимальная передача (высшая)

%% Входные данные двигателя
n_points = 43; % количество точек
RPM = linspace(1250, 6375, n_points); % обороты двигателя
Torque = [113.6 115.3 117.1 118.9 120.7 122 123.3 124.2 125.1 125.8 126.4 126.8 127.1 127.6 128 128.8 129.6 132.6 133.3 135.3 137.3 138.8 140.2 141.3 141.1 140.6 140 139.7 139.3 139.1 138.9 138 137.1 135.7 134.2 132.8 131.3 130.1 128.9 125.6 122.2 119.6 116.9]; % крутящий момент Нм
Power = (Torque .* RPM * 2 * pi / 60) / 1000; % мощность в кВт

%% Минимальные и максимальные обороты

RPM_min = RPM(1);
RPM_max = RPM(end);

Torque_min_RPM = Torque(1);         % момент при min оборотах
Torque_max = max(Torque);           % max момент
Torque_max_RPM = Torque(end);       % момент при max оборотах

%% Потери трансмиссии

function efficiency = func(i, M)

    global k_aux k_mt i_CVT_min i_CVT_max Torque_max %#ok<GVMIS>
    
    % Параметры перевернутого параболоида
    i0 = (i_CVT_min + i_CVT_max)/2; % центр параболы

    % Линейное изменение пика и краев по моменту M
    z_max_M = 0.88 + (0.92-0.88)/Torque_max .* M; % пик параболы
    z_min_M = 0.76 + (0.86-0.76)/Torque_max .* M; % крайние значения

    % Коэффициент параболы по M
    a_M = (z_max_M - z_min_M) ./ (i_CVT_max - i0).^2;

    % Перевернутый параболоид по i для каждого M
    fi = z_max_M - a_M .* (i - i0).^2;

    % Итоговый КПД с учетом потерь
    efficiency = k_aux * k_mt .* fi;
end

%% Визуализация поверхности КПД вариатора

i = linspace(i_CVT_min, i_CVT_max, n_points);
M = linspace(0, Torque_max, n_points);

[I, MESH] = meshgrid(i, M);

% Значения КПД вариатора
ETA = func(I, MESH)/(k_aux*k_mt);

figure;
surf(I, MESH, ETA);
xlabel('Передаточное число вариатора u');
ylabel('Момент M, Нм');
zlabel('КПД');
title('КПД вариатора \eta(u, M)');
shading interp;
colormap jet
colorbar;
zlim([0.73 0.95]);

%% Расчет главной передачи

% Поиск ГП по максимальной скорости
V_max = 3000; % м/мин 180 км/ч Максимальная скорость автомобиля
wheel_length = 2 * pi * wheel_radius;
wheel_RPM_max = V_max / wheel_length;

% определение оборотов двигателя на пиковой мощности
[~, max_power_point] = max(Power); % индекс точки пиковой мощности
engine_RPM_max_power = RPM(max_power_point);

final_drive = engine_RPM_max_power / (wheel_RPM_max * i_CVT_min);

disp(final_drive);

%% Внешняя скоростная характеристика

figure
title('Внешняя скоростная характеристика')
xlabel('Обороты двигателя, об/мин')
grid on

% --- Левая ось Y (Момент) ---
yyaxis left
plot(RPM, Torque, 'Color', blue, 'LineWidth', 2)
ylabel('Момент, Нм')
set(gca, 'YColor', blue);
ylim([105 145]);

% --- Правая ось Y (Мощность) ---
yyaxis right
plot(RPM, Power, 'Color', red, 'LineWidth', 2)
ylabel('Мощность, кВт')
set(gca, 'YColor', red);
ylim([0 85]);

legend('Момент', 'Мощность', 'Location', 'southeast')

%% Кривая сопротивлений движения автомобиля

% Скорость для сопротивлений (0..200 км/ч)
v_res_kmh = linspace(0, 200, n_points);
v_res = v_res_kmh / 3.6; % перевод в м/с

% Параметры сопротивлений
Cr = 0.015;      % коэффициент качения
Cd = 0.32;       % аэродинамика
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
eta_low  = func(i_CVT_max, Torque); % массив коэффициентов эффективности
v_low  = (2*pi*RPM/60) * wheel_radius / (i_CVT_max*final_drive) * 3.6;
F_low  = Torque .* eta_low .* i_CVT_max * final_drive / wheel_radius;

% Кривая тяги от скорости на низшей передаче
eta_high = func(i_CVT_min, Torque); % массив коэффициентов эффективности
v_high = (2*pi*RPM/60) * wheel_radius / (i_CVT_min*final_drive) * 3.6;
F_high = Torque .* eta_high .* i_CVT_min * final_drive / wheel_radius;

% Изменение тяги на низших оборотах от передаточного числа
eta_left = func(i_range, Torque_min_RPM); % массив коэффициентов эффективности
v_left  = (2*pi*RPM_min/60) * wheel_radius ./ (i_range*final_drive) * 3.6;
F_left  = Torque_min_RPM .* eta_left .* i_range * final_drive / wheel_radius;

% Изменение тяги на высших оборотах от передаточного числа
eta_right = func(i_range, Torque_max_RPM); % массив коэффициентов эффективности
v_right = (2*pi*RPM_max/60) * wheel_radius ./ (i_range*final_drive) * 3.6;
F_right = Torque_max_RPM .* eta_right .* i_range * final_drive / wheel_radius;

%% Построение тяговой характеристики

figure;
title('Тяго-динамическая характеристика CVT')
grid on;
xlabel('Скорость, км/ч')
ylabel('Тяга на колёсах, Н')

xlim([0 200]); % График до 200 км/ч

hold on;

% Левая граница (min RPM)
plot(v_left,  F_left,  'black--', 'LineWidth', 1);

% Правая граница (max RPM)
plot(v_right, F_right, 'black--', 'LineWidth', 1);

% Верхняя (низшая передача)
plot(v_low,  F_low, 'Color',  blue, 'LineWidth', 2);

% Нижняя (высшая передача)
plot(v_high, F_high, 'Color', green, 'LineWidth', 2);

% Сопротивление движению
plot(v_res_kmh, F_res, 'Color', red, 'LineWidth', 2);

hold off;

legend('Граница min обороты','Граница max обороты', ...
    'Низшая передача','Высшая передача', 'Сопротивление движению');

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
title('Динамическая характеристика')
grid on;
xlabel('Скорость, км/ч')
ylabel('Динамический фактор')

xlim([0 200]); % График до 200 км/ч

hold on;

% Левая граница (min RPM)
plot(v_left,  DF_left,  'black--', 'LineWidth', 1);

% Правая граница (max RPM)
plot(v_right, DF_right, 'black--', 'LineWidth', 1);

% Верхняя (низшая передача)
plot(v_low,   DF_low, 'Color',   blue, 'LineWidth', 2);

% Нижняя (высшая передача)
plot(v_high,  DF_high, 'Color',  green, 'LineWidth', 2);

% Предел сцепления
DF_traction = mu * AB;

plot([0 200], [DF_traction DF_traction], ...
    'Color', red, 'LineStyle', '-.', 'LineWidth', 1);
text(125, DF_traction + 0.015, ...
     sprintf('предел сцепления ДФ %.2f', DF_traction), ...
     'Color', red, 'FontSize', 9);

% Максимальные углы склона на высшей и низшей передачах
[DF_low_max, idx_low]   = max(DF_low);
[DF_high_max, idx_high] = max(DF_high);

alpha_low_deg  = asind(DF_low_max);
alpha_high_deg = asind(DF_high_max);

% Горизонтальная линия максимального уклона на низшей передаче
plot([0 200], [DF_low_max DF_low_max], ...
    'Color', blue, 'LineStyle', '--', 'LineWidth', 1);
text(40, DF_low_max + 0.015, ...
    sprintf('max уклон низшая: %.1f°', ...
    alpha_low_deg), ...
    'Color', blue, 'FontSize', 9);

% Горизонтальная линия максимального уклона на высшей передаче
plot([0 200], [DF_high_max DF_high_max], ...
    'Color', green, 'LineStyle', '--', 'LineWidth', 1);
text(135, DF_high_max + 0.015, ...
    sprintf('max уклон высшая: %.1f°', ...
    alpha_high_deg), ...
    'Color', green, 'FontSize', 9);

hold off;

legend('Граница min обороты','Граница max обороты', ...
    'Низшая передача','Высшая передача');

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
title('Продольное ускорение автомобиля')
grid on;
xlabel('Скорость, км/ч')
ylabel('Ускорение, м/с^2')
xlim([0 200])

hold on;

% Левая граница (min RPM)
plot(v_left,  a_left,  'black--', 'LineWidth', 1);

% Правая граница (max RPM)
plot(v_right, a_right, 'black--', 'LineWidth', 1);

% Низшая передача
plot(v_low,  a_low,  'Color', blue, 'LineWidth', 2);

% Высшая передача
plot(v_high, a_high, 'Color', green, 'LineWidth', 2);

% Предел сцепления (вертикальный потолок ускорения)
a_traction = mu * AB * g;
plot([0 200], [a_traction a_traction], 'Color', red, ...
    'LineWidth', 1, 'LineStyle','--');

text(110, a_traction + 0.15, ...
    sprintf('предел сцепления %.2f м/с²', a_traction), ...
    'Color', red, 'FontSize', 9);

hold off;

legend('Граница min обороты','Граница max обороты', ...
       'Низшая передача','Высшая передача');