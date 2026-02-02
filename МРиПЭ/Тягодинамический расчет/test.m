clc; clear; close all;

% Диапазоны
x_min = 0.427; x_max = 2.561;
y_min = 0; y_max = 141.3;
x0 = (x_min + x_max)/2;

% Сетка
[x, y] = meshgrid(linspace(x_min, x_max, 200), linspace(y_min, y_max, 200));

% Линейное изменение пика и краев по y
z_max_y = 0.88 + (0.92-0.88)/y_max * y;
z_min_y = 0.76 + (0.86-0.76)/y_max * y;

% Коэффициент параболы по y
a_y = (z_max_y - z_min_y) ./ (x_max - x0).^2;

% Поверхность
z = z_max_y - a_y .* (x - x0).^2;

% Построение
figure
surf(x, y, z)
xlabel('x')
ylabel('y')
zlabel('z')
title('Перевернутый параболоид с изменяющимся пиком по y')
shading interp
colormap jet
colorbar