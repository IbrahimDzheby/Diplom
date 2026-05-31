clc;
clear;
close all;

% загрузка таблицы из Excel
data = readmatrix('CVT.xlsx');

% столбцы
ratio  = data(:,1);   % передаточное отношение
torque = data(:,2);   % момент
eff    = data(:,3);   % КПД

% создаем регулярную сетку
ratio_grid  = linspace(0.4, 2.6, 100);
torque_grid = linspace(0, 180, 100);

[R,T] = meshgrid(ratio_grid, torque_grid);

% интерполяция КПД на сетку
E = griddata(ratio, torque, eff, R, T, 'cubic');

% построение поверхности
figure
surf(R, T, E)

hold on
scatter3(ratio, torque, eff, 10, 'k', 'filled')

set(gca, 'FontName', 'GOST Type A', 'FontAngle', 'italic', 'FontSize', 20);
xlabel('Передаточное отношение')
ylabel('Момент, Нм')
zlabel('КПД')
title('Поверхность эффективности вариатора', 'FontWeight', 'normal');

xlim([0.4 2.6])
ylim([0 180])
zlim([0.78 0.9])

shading interp
colormap(jet)
colorbar
grid on
view(45,30)