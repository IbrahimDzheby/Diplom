clear
clc
close all

%% Загрузка данных
M = readmatrix('BSFC.xlsx');
rpm    = M(:,1);
torque = M(:,2);
bsfc   = M(:,3);

%% Создаём сетку для интерполяции
[n, T] = meshgrid(linspace(0,5500,200), linspace(0,150,200));

%% Интерполяция поверхности (натуральная соседняя)
f = fit([rpm, torque], bsfc, 'poly35');
BSFC = f(n, T);

%% Контурная карта поверхности
figure
contourf(n, T, BSFC, 30, 'LineColor', 'none')  % фоновые контуры
hold on
contour(n, T, BSFC, 30, 'k', 'LineWidth', 0.8) % чёрные линии интерполяции

%% Наложение исходных контуров из таблицы красными линиями
uniqueLevels = unique(bsfc);
for i = 1:length(uniqueLevels)
    level = uniqueLevels(i);
    idx = bsfc == level;
    x = rpm(idx);
    y = torque(idx);
    
    % сортируем по углу обхода для красивой линии
    xc = mean(x); yc = mean(y);
    angles = atan2(y - yc, x - xc);
    [~, order] = sort(angles);
    
    plot(x(order), y(order), 'r-', 'LineWidth', 1)  % тонкие красные линии
end

xlabel('n (rpm)')
ylabel('T (Nm)')
title('BSFC map with original contours')
colorbar
grid on