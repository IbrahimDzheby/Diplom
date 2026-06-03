clc; clear; close all;

%% 1. Данные для s(SOC)
SOC_points = [0, 30, 60, 100];
s_points   = [0.193, 0.193, 0.077, 0.059];

SOC_fine = linspace(0, 100, 100);
s_custom = interp1(SOC_points, s_points, SOC_fine, 'linear', 'extrap');

%% 2. Построение графика
figure('Name', 'Функция топливного эквивалента s(SOC)', 'Color', 'w', ...
       'Position', [100, 100, 800, 500]);
plot(SOC_fine, s_custom, 'b-', 'LineWidth', 3); hold on;
plot(SOC_points, s_points, 'ro', 'MarkerSize', 10);
grid on;

%% 3. Настройка шрифта и размера (ГОСТ Type A, одинаковый размер)
set(gca, 'FontName', 'GOST Type A', 'FontAngle', 'italic', 'FontSize', 22);
xlabel('SOC, %');
ylabel('s, г/кВтс');
title('Функция топливного эквивалента s(SOC)', 'FontWeight', 'normal');

hold off;