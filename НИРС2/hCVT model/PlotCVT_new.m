clc; close all; clear;

% Чтение и подготовка данных (как раньше)
data = readtable('CVT.xlsx', 'VariableNamingRule', 'preserve');
A = data{:,1}; B = data{:,2}; C = data{:,3};
i_cvt_list = unique(A, 'stable');   % 22×1
T_in_list  = unique(B, 'stable');   % 19×1
eta_grid   = reshape(C, length(T_in_list), length(i_cvt_list)); % 19×22

% Регулярный 2D-интерполянт в координатах (i_cvt, T_in)
[I_grid, T_in_grid] = ndgrid(i_cvt_list, T_in_list);  % для interpn нужна сетка ndgrid
F_eta = griddedInterpolant(I_grid, T_in_grid, eta_grid', 'linear', 'none'); 
% Аргументы: (i_cvt, T_in) → η

% Функция для вычисления η по (i_cvt, T_out)
function [eta_val, T_in_found] = getEta(i_cvt, T_out, F_eta, T_in_limits)
    % Решаем: i_cvt * T_in * F_eta(i_cvt, T_in) - T_out = 0
    cost = @(T_in) i_cvt * T_in * F_eta(i_cvt, T_in) - T_out;
    % Предполагаем, что решение лежит внутри границ T_in
    [T_in_found, ~, exitflag] = fzero(cost, T_in_limits);
    if exitflag ~= 1
        % Если не сошлось, возможно, точка вне области определения
        eta_val = NaN;
        T_in_found = NaN;
    else
        eta_val = F_eta(i_cvt, T_in_found);
    end
end

% --- Проверка на тех же точках, что вы привели ---
% Парсим вашу таблицу (приблизительно)
% Предположим, это точки с постоянным T_out = 100 и i_cvt от 0.8 до 1.21 с шагом 0.01
% (если ваш формат другой – подставьте свои векторы)
i_test = (0.6:0.1:2.3)';
T_out_test = 17 * ones(size(i_test)); 
eta_test = zeros(size(i_test));
T_in_test = zeros(size(i_test));
for k = 1:length(i_test)
    [eta_test(k), T_in_test(k)] = getEta(i_test(k), T_out_test(k), F_eta, ...
                                          [min(T_in_list), max(T_in_list)]);
end

% Визуализация: накладываем на поверхность surf
figure('Name','Проверка надёжного метода')
% Поверхность через surf (как раньше)
T_in_mat = repmat(T_in_list, 1, length(i_cvt_list));
i_cvt_mat = repmat(i_cvt_list', length(T_in_list), 1);
T_out_mat = i_cvt_mat .* T_in_mat .* eta_grid;
surf(i_cvt_mat, T_out_mat, eta_grid, 'EdgeColor','interp','FaceColor','interp')
xlabel('i_{cvt}'), ylabel('T_{cvt,out}'), zlabel('\eta')
hold on
% Точки по новому методу
plot3(i_test, T_out_test, eta_test, 'r.', 'MarkerSize', 20)
title('Поверочные точки (красные) точно на поверхности')
colorbar, view(45,30), grid on
hold off