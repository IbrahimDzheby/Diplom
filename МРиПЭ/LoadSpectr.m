clc; clear; close all;

%% -------------------------------
% ПАРАМЕТРЫ
%% -------------------------------
V_max = 180;
dV = 1;              
V = 0:dV:V_max;

V_mean = 47.6;
sigma = 20;

R_wheel = 0.285;

i_final = 5.232;
i_cvt_max = 2.561;
i_cvt_min = 0.427;

n_engine_opt = 2000;
n_engine_min = 600;
n_engine_max = 5400;

m = 1500; 
g = 9.81;
Crr = 0.015;
rho = 1.2; 
Cd = 0.3; 
A = 2.2;

%% -------------------------------
% 1. НОРМАЛЬНОЕ РАСПРЕДЕЛЕНИЕ
%% -------------------------------
pdf_V = (1/(sigma*sqrt(2*pi))) * exp(-0.5*((V - V_mean)/sigma).^2);
pdf_V = pdf_V / sum(pdf_V);

V_ms = V / 3.6;
n_wheel = (V_ms ./ (2*pi*R_wheel)) * 60;

%% -------------------------------
% 2. CVT (держим n_engine_opt)
%% -------------------------------
i_cvt = n_engine_opt ./ (n_wheel + 1e-6) / i_final;
i_cvt = min(max(i_cvt, i_cvt_min), i_cvt_max);

i_total = i_cvt * i_final;
n_engine = n_wheel .* i_total;

%% -------------------------------
% 3. Силы и момент
%% -------------------------------
F_total = m*g*Crr + 0.5*rho*Cd*A.*(V_ms.^2);
T_wheel = F_total * R_wheel;
T_engine = T_wheel ./ i_total;

%% -------------------------------
% 4. Мощность двигателя
%% -------------------------------
P_engine = T_engine .* (2*pi .* n_engine / 60);   % Ватты

%% -------------------------------
% 5. Квантуем по скоростям (шаг 5 км/ч)
%% -------------------------------
V_step = 5;
V_edges = 0:V_step:V_max;
num_steps = length(V_edges) - 1;

p_step = zeros(num_steps,1);
P_max_step = zeros(num_steps,1);
n_engine_step = zeros(num_steps,1);
V_mid = zeros(num_steps,1);

for k = 1:num_steps
    
    if k < num_steps
        idx = V >= V_edges(k) & V < V_edges(k+1);
    else
        idx = V >= V_edges(k) & V <= V_edges(k+1);
    end
    
    if any(idx)
        p_step(k) = sum(pdf_V(idx));
        P_max_step(k) = max(P_engine(idx));       % максимальная мощность
        n_engine_step(k) = mean(n_engine(idx));   % средние обороты
    else
        p_step(k) = 0;
        P_max_step(k) = 0;
        n_engine_step(k) = 0;
    end
    
    V_mid(k) = (V_edges(k) + V_edges(k+1)) / 2;
end

%% -------------------------------
% 6. Подготовка данных
%% -------------------------------
p_percent = p_step;                   % частота
n_engine_col = n_engine_step/1000;          % обороты
P_kW = P_max_step / (24000);              % мощность в кВт

%% -------------------------------
% 7. Таблица для KISSsoft
%% -------------------------------
result = table(p_percent, n_engine_col, P_kW, ...
    'VariableNames', {'Frequency_percent', 'Engine_rpm', 'Power_kW'});

writetable(result, 'load.xlsx');
disp('Готово. Таблица сохранена в load.xlsx');

%% -------------------------------
% 8. Графики
%% -------------------------------

% Распределение ресурса
figure;
bar(V_mid, p_percent);
grid on;
xlabel('Скорость (км/ч)');
ylabel('Частота (%)');
title('Распределение ресурса по скоростям');

% Мощность по скоростям
figure;
bar(V_mid, P_kW);
grid on;
xlabel('Скорость (км/ч)');
ylabel('Мощность (кВт)');
title('Максимальная мощность по скоростям');