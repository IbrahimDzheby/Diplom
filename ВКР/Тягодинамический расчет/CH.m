clc; clear; close all;

% % Исходные данные МГ
% RPM = [0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5200];
% TorquePeak = [88, 88, 88, 88, 88, 88, 88, 80, 72, 65, 59, 57];
% Torque = [41, 42, 43, 43.75, 44.5, 45, 45.5, 45.75, 42.5, 38, 34.5, 33.5];
% PowerPeak = (TorquePeak .* RPM * 2 * pi / 60) / 1000;
% Power = (Torque .* RPM * 2 * pi / 60) / 1000;
% 
% % Построение графика
% figure;
% yyaxis left;
% plot(RPM, Torque, '-b', 'LineWidth', 2);
% hold on;
% plot(RPM, TorquePeak, '--b', 'LineWidth', 2);
% ylabel('Момент, Н·м');
% set(gca, 'YColor', 'b');   % левая ось — синяя
% 
% yyaxis right;
% plot(RPM, Power, '-r', 'LineWidth', 2);
% plot(RPM, PowerPeak, '--r', 'LineWidth', 2);
% ylabel('Мощность, кВт');
% set(gca, 'YColor', 'r');   % правая ось — красная
% 
% xlabel('Обороты двигателя, об/мин');
% grid on;
% 
% legend('Момент (ном.)', 'Момент (пик.)', 'Мощность (ном.)', 'Мощность (пик.)', ...
%        'Location', 'best');
% 
% xlim([0 5200]);
% hold off;

%% Входные данные ДВС
n_points = 13;
 
RPM = [800, 1113.39, 1614.81, 2015.95, 2404.56, 3006.27, 3231.91, 3607.98, 4009.12, 4372.65, 4686.04, 4986.89, 5200];
Torque = [101.10, 106.59, 116.48, 123.08, 131.32, 136.81, 138.46, 141.76, 142.86, 140.11, 137.91, 136.26, 134.07]; % крутящий момент Нм
Power = (Torque .* RPM * 2 * pi / 60) / 1000; % мощность в кВт

figure;
yyaxis left;
plot(RPM, Torque, '-b', 'LineWidth', 2);
hold on;
ylabel('Момент, Н·м');
set(gca, 'YColor', 'b');   % левая ось — синяя

yyaxis right;
plot(RPM, Power, '-r', 'LineWidth', 2);
ylabel('Мощность, кВт');
set(gca, 'YColor', 'r');   % правая ось — красная

xlabel('Обороты двигателя, об/мин');
grid on;

legend('Момент', 'Мощность');

xlim([500 5500]);
yyaxis left;  ylim([95 150]);
yyaxis right; ylim([0 85]); 
hold off;