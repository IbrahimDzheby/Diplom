clc; close all; clear;

% --- 1. Загрузка и подготовка интерполянта (один раз) ---
data = readtable('CVT.xlsx', 'VariableNamingRule', 'preserve');
i_all = data{:,1};
T_all = data{:,2};
eta_all = data{:,3};

i_vec = unique(i_all, 'stable');
T_vec = unique(T_all, 'stable');
eta_grid = reshape(eta_all, length(T_vec), length(i_vec))';   % 22×19

F_eta = griddedInterpolant({i_vec, T_vec}, eta_grid, 'linear', 'none');

% --- 2. Функция-обёртка (вся логика внутри, аргументы – F_eta и T_vec) ---
% Просто вызывайте: eta = CVT_map(i, T_out, F_eta, T_vec)
function eta = CVT_map(i_val, T_out_val, F_eta, T_vec)
    % Поиск КПД по i_cvt и выходному моменту
    % Уравнение: i * T_in * eta(i, T_in) - T_out = 0
    cost = @(T_in) i_val * T_in * F_eta(i_val, T_in) - T_out_val;
    options = optimset('Display','off');   % без лишних сообщений
    try
        T_sol = fzero(cost, [min(T_vec), max(T_vec)], options);
        eta = F_eta(i_val, T_sol);
    catch
        eta = NaN;
    end
end

% --- 3. Примеры и проверка ---
i_query = [0.6; 1.0; 1.5; 2.2];
T_out_query = [40; 80; 140; 250];
eta_result = arrayfun(@(i,t) CVT_map(i, t, F_eta, T_vec), i_query, T_out_query);
disp(table(i_query, T_out_query, eta_result))

% Визуальная проверка (как раньше)
figure
[T_in_grid, i_grid] = meshgrid(T_vec, i_vec);
T_out_grid = i_grid .* T_in_grid .* eta_grid;
surf(i_grid, T_out_grid, eta_grid, 'EdgeColor','interp','FaceColor','interp');
xlabel('i_{cvt}'), ylabel('T_{cvt,out}'), zlabel('\eta')
hold on
plot3(i_query, T_out_query, eta_result, 'r.', 'MarkerSize', 25);
legend('Поверхность','Запросы')
hold off