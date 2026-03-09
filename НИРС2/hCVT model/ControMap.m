clear
clc
close all

%% загрузка Excel
M = readmatrix('BSFC.xlsx');

bsfc   = M(:,1);
rpm    = M(:,2);
torque = M(:,3);

%% создаём сетку
[n,T] = meshgrid(linspace(0,5500,200), ...
                 linspace(0,150,200));

%% АППРОКСИМАЦИЯ ПОВЕРХНОСТИ (вместо scatteredInterpolant)

f = fit([rpm, torque], bsfc, 'poly55');

BSFC = f(n,T);

%% 3D поверхность
figure
surf(n,T,BSFC,'EdgeColor','none')
xlabel('n (rpm)')
ylabel('T (Nm)')
zlabel('BSFC')
title('BSFC surface')
colorbar
view(45,30)
shading interp

%% контурная карта
figure
contourf(n,T,BSFC,75)
xlabel('n (rpm)')
ylabel('T (Nm)')
title('BSFC map')
colorbar