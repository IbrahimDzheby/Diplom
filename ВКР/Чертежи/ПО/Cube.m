clear; close all; clc;

% --- Единый шрифт ГОСТ Type A (наклонный) ---
set(0, 'DefaultTextFontSize', 22);
set(0, 'DefaultTextFontName', 'GOST Type A');
set(0, 'DefaultTextFontAngle', 'italic');
set(0, 'DefaultAxesFontSize', 22);
set(0, 'DefaultAxesFontName', 'GOST Type A');
set(0, 'DefaultAxesFontAngle', 'italic');

% --- Размер сетки ---
n = 4;                         % 5×5×5 = 125 точек
coords = linspace(0, 1, n);

% --- Узлы сетки ---
[X, Y, Z] = meshgrid(coords, coords, coords);
x_vec = X(:);
y_vec = Y(:);
z_vec = Z(:);

% --- Построение фигуры ---
figure('Color', 'w');
hold on;

% 1. Внутренние линии (исключая ВСЕ рёбра)
%    Линии вдоль X
for j = 1:n
    for k = 1:n
        if (j == 1 || j == n) && (k == 1 || k == n)
            continue;
        end
        plot3(coords, coords(j)*ones(1,n), coords(k)*ones(1,n), ...
              'Color', [0.85 0.85 0.85], 'LineWidth', 0.01, 'HandleVisibility', 'off');
    end
end

%    Линии вдоль Y
for i = 1:n
    for k = 1:n
        if (i == 1 || i == n) && (k == 1 || k == n)
            continue;
        end
        plot3(coords(i)*ones(1,n), coords, coords(k)*ones(1,n), ...
              'Color', [0.85 0.85 0.85], 'LineWidth', 0.01, 'HandleVisibility', 'off');
    end
end

%    Линии вдоль Z
for i = 1:n
    for j = 1:n
        if (i == 1 || i == n) && (j == 1 || j == n)
            continue;
        end
        plot3(coords(i)*ones(1,n), coords(j)*ones(1,n), coords, ...
              'Color', [0.85 0.85 0.85], 'LineWidth', 0.01, 'HandleVisibility', 'off');
    end
end

% 2. Синие точки в узлах
scatter3(x_vec, y_vec, z_vec, 10, 'b', 'filled', ...
         'MarkerEdgeColor', 'b', 'MarkerFaceColor', 'b');

% 3. Основной контур куба (box on) – все 12 рёбер чёрные
box on;

% 4. Три выбранных ребра – рисуем ПОВЕРХ чёрных, серым цветом (стиль сетки)
%    Ребро 1: (SOC=0, h=0, V)
plot3([0 0], [0 0], [0 1], 'Color', [0.85 0.85 0.85], 'LineWidth', 0.01, 'HandleVisibility', 'off');
%    Ребро 2: (SOC=0, h, V=120)
plot3([0 0], [0 1], [1 1], 'Color', [0.85 0.85 0.85], 'LineWidth', 0.01, 'HandleVisibility', 'off');
%    Ребро 3: (SOC, h=0, V=120)
plot3([0 1], [0 0], [1 1], 'Color', [0.85 0.85 0.85], 'LineWidth', 0.01, 'HandleVisibility', 'off');

% --- Настройка осей ---
xlabel('Ёмкость батареи, SoC %');
ylabel('Ход педали, h %');
zlabel('Скорость, V км/ч');
title('Пространство рабочик точек', 'FontWeight', 'normal');

xlim([0 1]); ylim([0 1]); zlim([0 1]);
daspect([1 1 1]);
grid off;   % отключаем стандартную сетку

% --- Метки реальных значений ---
soc_min = 0;   soc_max = 100;
pedal_min = 0; pedal_max = 100;
v_min = 0;     v_max = 120;

xticks([0 1]);
yticks([0 1]);
zticks([0 1]);
xticklabels({sprintf('%.0f', soc_min), sprintf('%.0f', soc_max)});
yticklabels({sprintf('%.0f', pedal_min), sprintf('%.0f', pedal_max)});
zticklabels({sprintf('%.0f', v_min), sprintf('%.0f', v_max)});

% set(gca, 'XTick', [], 'YTick', [], 'ZTick', []);

view(45, 30);
hold off;