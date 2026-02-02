clc;
clear;
close all;

%% Параметры модели
eta_max = 0.92;     % максимальный КПД
eta_min = 0.75;     % минимальный КПД
a = 0.10;           % наклон по моменту
b = 0.07;           % наклон по передаточному числу

M_max = 150;        % максимальный момент двигателя, Н·м
i_min = 0.427;        % минимальное передаточное число
i_max = 2.561;        % максимальное передаточное число
i_opt = sqrt(i_min*i_max);  % оптимальное i для максимального КПД

%% Создаём сетку значений M и i
M = linspace(0, M_max, 50);          % момент
i = linspace(i_min, i_max, 50);      % передаточное число
[M_grid, i_grid] = meshgrid(M, i);

%% Расчёт КПД
eta_grid = eta_max - a*(M_grid/M_max) - b*abs(log(i_grid/i_opt));

% Ограничиваем диапазон КПД
eta_grid = max(eta_min, min(eta_max, eta_grid));

%% Построение 3D-графика
figure;
surf(M_grid, i_grid, eta_grid)
xlabel('Момент, Н·м')
ylabel('Передаточное число i')
zlabel('КПД вариатора')
title('Поверхность КПД вариатора')
colorbar
shading interp
view(45,30)
grid on
