%% Plot_BSFC_FINAL_v4.m
% Финальная версия: 3D поверхность + изолинии из Excel + кривая момента
clear; close all; clc;

%% 1. Загрузка данных
fprintf('Загрузка данных...\n');

% Загрузка поверхности для 3D
if exist('BSFC_surface.mat', 'file')
    load('BSFC_surface.mat');
    fprintf('- Загружена поверхность: BSFC_surface.mat\n');
else
    error('Файл BSFC_surface.mat не найден');
end

% Загрузка изолиний из Excel
if exist('BSFC.xlsx', 'file')
    bsfc_lines = readmatrix('BSFC.xlsx');
    rpm_lines = bsfc_lines(:, 1);     % Обороты для точек изолиний
    torque_lines = bsfc_lines(:, 2);   % Момент для точек изолиний
    bsfc_values = bsfc_lines(:, 3);    % Значения BSFC для каждой точки
    fprintf('- Загружены изолинии: BSFC.xlsx (%d точек)\n', length(bsfc_values));
else
    error('Файл BSFC.xlsx не найден');
end

% Загрузка кривой максимального момента
if exist('Tmax.xlsx', 'file')
    Tmax_data = readmatrix('Tmax.xlsx');
    rpm_max = Tmax_data(:, 1);
    torque_max = Tmax_data(:, 2);
    fprintf('- Загружена кривая момента: Tmax.xlsx\n');
else
    error('Файл Tmax.xlsx не найден');
end

fprintf('Все данные загружены.\n\n');

%% 2. ТРЕХМЕРНЫЙ ГРАФИК (полная поверхность)
figure('Name', '3D Поверхность BSFC', 'NumberTitle', 'off', 'Color', 'w');
set(gcf, 'Position', [100, 100, 900, 700]);

% Извлекаем координаты из вершин
rpm_3d = all_vertices(:, 1);
torque_3d = all_vertices(:, 2);
bsfc_3d = all_vertices(:, 3);

% Строим поверхность с черными треугольниками
trisurf(all_triangles, rpm_3d, torque_3d, bsfc_3d, ...
    'EdgeColor', 'k', ...      % Черные контуры треугольников
    'FaceColor', 'interp', ... % Интерполированная заливка
    'LineWidth', 0.3);

colormap(jet); % Радужная цветовая схема
colorbar;

set(gca, 'FontName', 'GOST Type A', 'FontAngle', 'italic', 'FontSize', 22);

xlabel('Обороты, об/мин');
ylabel('Момент, Н·м');
zlabel('BSFC, г/кВт·ч');
title('Поверхность BSFC', 'FontWeight', 'normal');

xlim([800, 5200]);
ylim([0, 150]);

% Хороший угол обзора
view(135, 30);
grid on;
box on;

fprintf('3D график построен.\n');

%% 3. КОНТУРНЫЙ ГРАФИК (изолинии из Excel + кривая момента)
figure('Name', 'Изолинии BSFC и кривая момента', 'NumberTitle', 'off', 'Color', 'w');
set(gcf, 'Position', [950, 100, 900, 700]);

% Получаем уникальные значения BSFC для группировки точек в изолинии
unique_bsfc = unique(bsfc_values);
unique_bsfc = sort(unique_bsfc);

fprintf('Построение изолиний для %d уровней BSFC:\n', length(unique_bsfc));

% Создаем радужную цветовую карту для изолиний
colors = jet(length(unique_bsfc));

hold on;

% Для каждого уникального значения BSFC рисуем свою изолинию
for i = 1:length(unique_bsfc)
    level = unique_bsfc(i);
    
    % Находим все точки с этим значением BSFC
    level_idx = abs(bsfc_values - level) < 1e-6;
    rpm_level = rpm_lines(level_idx);
    torque_level = torque_lines(level_idx);
    
    % Сортируем точки по полярному углу для правильного обхода изолинии
    if length(rpm_level) > 2
        center_rpm = mean(rpm_level);
        center_torque = mean(torque_level);
        angles = atan2(torque_level - center_torque, rpm_level - center_rpm);
        [~, sort_idx] = sort(angles);
        rpm_level = rpm_level(sort_idx);
        torque_level = torque_level(sort_idx);
        % Замыкаем контур
        rpm_level(end+1) = rpm_level(1);
        torque_level(end+1) = torque_level(1);
        
        % Рисуем изолинию С ОТКЛЮЧЁННОЙ ВИДИМОСТЬЮ В ЛЕГЕНДЕ
        plot(rpm_level, torque_level, 'Color', colors(i, :), 'LineWidth', 2, ...
            'HandleVisibility', 'off');
        
        fprintf('  Уровень %d: %d точек\n', round(level), length(rpm_level)-1);
    elseif length(rpm_level) == 2
        plot(rpm_level, torque_level, 'Color', colors(i, :), 'LineWidth', 2, ...
            'HandleVisibility', 'off');
        fprintf('  Уровень %d: 2 точки (линия)\n', round(level));
    end
end

% --- Фиктивный объект для изолиний BSFC (символ в легенде) ---
h_bsfc = plot(NaN, NaN, 'LineWidth', 2, 'Color', [0 0 1], ...  % синяя линия
              'DisplayName', 'BSFC двигателя (изолинии)');

% --- Кривая максимального момента (сохраняем хэндл) ---
h_moment = plot(rpm_max, torque_max, 'k-', 'LineWidth', 3, ...
                'DisplayName', 'Макс. момент ДВС');

% --- Фиктивный объект для рабочих точек ДВС (красные точки) ---
h_work = plot(NaN, NaN, 'r.', 'MarkerSize', 15, ...
              'DisplayName', 'Рабочие точки ДВС');

hold off;

xlabel('Обороты, об/мин', 'FontSize', 12);
ylabel('Момент, Н·м', 'FontSize', 12);
title('Изолинии BSFC и кривая максимального момента', 'FontSize', 14);

xlim([800, 5200]);
ylim([0, 150]);

grid on;
box on;

% Легенда только для трёх нужных элементов
legend([h_bsfc, h_moment, h_work], 'Location', 'best');

%% 4. Вывод статистики
fprintf('\n--- Статистика ---\n');
fprintf('3D поверхность: %d вершин, %d треугольников\n', size(all_vertices, 1), size(all_triangles, 1));
fprintf('Изолинии: %d уровней, всего %d точек\n', length(unique_bsfc), length(bsfc_values));
fprintf('Кривая момента: %d точек\n', length(rpm_max));
fprintf('\nГотово!\n');