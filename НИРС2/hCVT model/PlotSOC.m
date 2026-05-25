%% Генерация SOC путём масштабирования потребной мощности
% SOC = 60% - a * P_wheel, где a подобрано так, чтобы SOC_max = 67%
% (при максимальной рекуперации / минимальной мощности)

clear; close all; clc;

%% 1. Загрузка цикла WLTC
data = readtable('WLTP_road.xlsx');
t = data.Time;
v_kmh = data.Speed;
v = v_kmh / 3.6;
dt = [0; diff(t)];

%% 2. Параметры автомобиля (для расчёта мощности)
vehicle.m = 1600;
vehicle.r = 0.285;
vehicle.Cr = 0.015;
vehicle.Cd = 0.32;
vehicle.A = 2.2;
vehicle.rho = 1.225;
vehicle.g = 9.81;

%% 3. Расчёт потребной мощности на колёсах P_wheel (кВт)
N = length(t);
P_wheel = zeros(N,1);
for k = 2:N
    ak = (v(k) - v(k-1)) / max(dt(k), 0.001);
    F_roll = vehicle.m * vehicle.g * vehicle.Cr;
    F_aero = 0.5 * vehicle.rho * vehicle.Cd * vehicle.A * v(k)^2;
    F_inertia = vehicle.m * ak;
    F_total = F_roll + F_aero + F_inertia;
    P_wheel(k) = F_total * v(k) / 1000;   % кВт
end

% Ограничиваем рекуперацию мощностью МГ (~15 кВт)
P_regen_max = 15;
P_wheel(P_wheel < -P_regen_max) = -P_regen_max;

% Небольшое сглаживание мощности, чтобы убрать артефакты
P_wheel = smoothdata(P_wheel, 'gaussian', 5);

%% 4. Масштабирование: SOC = 60% - a * P_wheel
SOC_target_max = 0.67;   % желаемый пиковый SOC (при самой отрицательной мощности)
SOC_mid = 0.60;           % средний SOC при нулевой мощности

% Находим минимальную мощность (наибольшая рекуперация)
P_min = min(P_wheel);    % отрицательное число

% Вычисляем коэффициент a так, чтобы при P = P_min получилось SOC_max
% SOC_max = 0.60 - a * P_min  =>  a = (0.60 - SOC_max) / P_min
a = (SOC_mid - SOC_target_max) / P_min;   % a > 0, т.к. P_min < 0

% Получаем сырой SOC
SOC_raw = SOC_mid - a * P_wheel;

% Ограничиваем разумными пределами (чтобы не уходило в 0 или 1)
SOC_raw = min(max(SOC_raw, 0.55), 0.69);

% Дополнительное сглаживание (окно 5 секунд)
window_sec = 5;
window_size = round(window_sec / mean(dt(2:end)));
SOC_smooth = movmean(SOC_raw, window_size);

% Фиксируем конечное значение равным начальному 60% (коррекция дрейфа)
SOC_end = SOC_smooth(end);
drift = linspace(0, 0.60 - SOC_end, N)';
SOC_final = SOC_smooth + drift;
SOC_final = min(max(SOC_final, 0.55), 0.69);

%% 5. Построение графика
figure('Color','w','Position',[100 100 1000 600]);

plot(t, SOC_final*100, 'b-', 'LineWidth', 1.8);
xlabel('Время, с');
ylabel('SOC, %');
grid on;
ylim([54 67.4]);

set(gcf, 'Units', 'inches', 'Position', [1, 1, 12, 4]);
% Настройка шрифта для осей, подписей и заголовка
set(gca, 'FontName', 'Times New Roman', 'FontSize', 20);
xlabel('Время, с', 'FontName', 'Times New Roman', 'FontSize', 20);
ylabel('SOC, %', 'FontName', 'Times New Roman', 'FontSize', 20);