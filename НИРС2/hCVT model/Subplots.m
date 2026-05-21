%% plot_extra_curves.m
% Построение s-кривой и карты КПД электродвигателя

clear; clc; close all;

%% 1. Параметры для s-кривой (должны совпадать с основным скриптом)
hybrid.s_mid      = 0.07;
hybrid.A_s        = 0.04;
hybrid.k_s        = 15;
hybrid.SOC_target = 0.60;

%% 2. Построение s(SOC)
SOC_vals = linspace(0, 1, 200);
s_vals = arrayfun(@(soc) compute_s(soc, hybrid), SOC_vals);

figure('Name', 's-кривая ECMS');
plot(SOC_vals, s_vals, 'b-', 'LineWidth', 2);
xlabel('SOC (0..1)');
ylabel('Эквивалентный фактор s');
title('Зависимость эквивалентного фактора от SOC');
grid on;
hold on;
% Отметим целевую точку
plot(hybrid.SOC_target, compute_s(hybrid.SOC_target, hybrid), 'ro', 'MarkerSize', 8, 'LineWidth', 1.5);
legend('s(SOC)', 'Целевой SOC', 'Location', 'best');
ylim([0, 0.12]); % типичный диапазон

%% 3. Параметры для карты КПД электродвигателя
% Диапазон оборотов (об/мин)
rpm_vec = linspace(0, 5200, 100);
% Диапазон моментов (Н·м) – положительные, режим мотора
torque_vec = linspace(0, 89, 50);

% Переводим обороты в угловую скорость (рад/с)
w_vec = rpm_vec * (2*pi/60);

% Создаём сетку
[RPM, TORQUE] = meshgrid(rpm_vec, torque_vec);
W = RPM * (2*pi/60); % угловая скорость на сетке

% Вычисляем КПД для каждой точки сетки (режим 'motor')
eta_grid = zeros(size(W));
for i = 1:size(W,1)
    for j = 1:size(W,2)
        w_ij = W(i,j);
        T_ij = TORQUE(i,j);
        if w_ij < 0.1 || T_ij < 0.01
            eta_grid(i,j) = 0.5;   % низкий КПД на малых нагрузках/оборотах
        else
            eta_grid(i,j) = MG_efficiency(w_ij, T_ij, 'motor');
        end
    end
end

%% 4. Построение карты КПД
figure('Name', 'КПД электродвигателя (моторный режим)');
contourf(RPM, TORQUE, eta_grid, 20, 'LineColor', 'none');
colorbar;
xlabel('Обороты, об/мин');
ylabel('Крутящий момент, Н·м');
title('КПД электродвигателя (режим мотора)');
colormap(jet);
caxis([0.5, 0.97]);

% Наложим изолинии КПД
hold on;
[C, h] = contour(RPM, TORQUE, eta_grid, [0.7 0.8 0.85 0.9 0.95], 'k-', 'LineWidth', 1);
clabel(C, h, 'FontSize', 8);

% Добавим кривую максимального момента (по точкам из условия)
mg_rpm_curve = [0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000];
mg_torque_curve = [89, 88, 88, 88, 88, 88, 88, 80, 72, 65, 59];
plot(mg_rpm_curve, mg_torque_curve, 'b--', 'LineWidth', 2);
legend('КПД', '','Макс. момент', 'Location', 'northeast');
grid on;

%% Дополнительно: зависимость максимального момента от оборотов
figure('Name', 'Максимальный момент МГ');
plot(mg_rpm_curve, mg_torque_curve, 'ro-', 'LineWidth', 1.5);
xlabel('Обороты, об/мин');
ylabel('Максимальный момент, Н·м');
title('Ограничение по моменту электродвигателя');
grid on;
xlim([0, 5200]);
ylim([50, 100]);

%% ========== Локальные копии функций (или можно вызывать из внешних файлов) ==========
function s = compute_s(SOC, hybrid)
    s = hybrid.s_mid - hybrid.A_s * tanh(hybrid.k_s * (SOC - hybrid.SOC_target));
end

function eta = MG_efficiency(w, T, mode)
    % Параметры потерь (те же, что в основном коде)
    k_c = 0.005;
    k_i = 2.0;
    k_w = 0.1;
    C   = 50;
    
    if abs(T) < 0.01 || w < 1.0
        eta = 0.5;
        return;
    end
    
    P_mech = abs(T) * w;   % Вт
    P_loss = k_c * T^2 + k_i * w + k_w * w^1.5 + C;
    
    if strcmpi(mode, 'motor')
        eta = P_mech / (P_mech + P_loss);
    else
        eta = (P_mech - P_loss) / P_mech;
    end
    
    eta = min(max(eta, 0.5), 0.97);
end