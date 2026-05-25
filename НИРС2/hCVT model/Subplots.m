%% 1. Настройка пяти пользовательских точек для s(SOC) кривой
SOC_points = [0, 0.3, 0.5, 0.6, 1];
s_points   = [0.11, 0.1, 0.07, 0.02, 0.015];

%% 2. Построение кастомной s(SOC) зависимости
SOC_fine = linspace(0, 1, 500);
s_custom = interp1(SOC_points, s_points, SOC_fine, 'linear');

% --- График ---
figure('Name', 'Кастомная s(SOC) зависимость', 'Color', 'w', 'Position', [100, 100, 560, 420]);
plot(SOC_fine, s_custom, 'b-', 'LineWidth', 1.5); hold on;
plot(SOC_points, s_points, 'ro', 'MarkerSize', 8, 'LineWidth', 1.2); % Точки управления

% Настройка внешнего вида

set(gcf, 'Units', 'inches', 'Position', [1, 1, 12, 4]);
xlabel('SOC (0..1)', 'FontName', 'Times New Roman', 'FontSize', 20);
ylabel('Эквивалентный фактор s', 'FontName', 'Times New Roman', 'FontSize', 20);
grid on;