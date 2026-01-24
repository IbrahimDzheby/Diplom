clear; clc; close all;

% =========================
%      Исходные данные
% =========================

% Hybrid Synergy Drive
% 
% Toyota Prius 1.8 (2022)
% Расход топлива (город/шоссе/смешанный): 4,4/4,7/4,5 л/100км
% Разгон 60–100 км/ч: 7,4 с
% Аналог на ДВС: Toyota Corolla 1.8 (2022) — 7,8/6,2/7,1 л/100км
% 
% Toyota Camry Hybrid LE 2.5 (2024)
% Расход топлива: 4,6/4,4/4,5 л/100 км
% Разгон 60–100 км/ч: 4,9 с
% Аналог на ДВС: Toyota Camry LE 2.5 (2024) — 8,4/6,0/7,3 л/100км
% 
% Ford Fusion Hybrid 2.0 (2020)
% Расход топлива: 5,5/5,7/5,6 л/100 км
% Разгон 60–100 км/ч: 5,2 с
% Аналог на ДВС: Ford Fusion 2.0 (2020) — 11,2/7,6/9,4 л/100 км
% 
% Inline HEV
% 
% BMW 530d Hybrid 3.0 (2024)
% Расход топлива (город/трасса/смешанный): 6,2/4,9/5,6 л/100км
% Разгон 60–100 км/ч: 3,2 с
% Аналог на ДВС: BMW 530d 3.0 (2018) — 7,0/5,4/6,3 л/100км
% 
% Hyundai Sonata Hybrid 2.0 (2020)
% Расход топлива: 5,2/4,6/5,0 л/100 км
% Разгон 60–100 км/ч: 5,4 с
% Аналог на ДВС: Hyundai Sonata 2.0 (2019) — 10,2/7,3/9,0 л/100км
% 
% Hyundai Elantra Hybrid 1.6 (2021)
% Расход топлива: 4,8/4,5/4,7 л/100 км
% Разгон 60–100 км/ч: 8,1 с
% Аналог на ДВС: Hyundai Elantra 1.6 (2020) — 9,0/7,1/8,1 л/100км
% 
% MM-Series HEV
% 
% Honda Accord Hybrid 2.0 (2020)
% Расход топлива: 4,9/5,0/4,9 л/100км
% Разгон 60–100 км/ч: 5,5 с
% Аналог на ДВС: Honda Accord 2.0 (2020) — 10,2/6,9/8,7 л/100км
% 
% Honda Civic Hybrid 1.5 (2015)
% Расход топлива: 5,5/5,2/5,3 л/100 км
% Разгон 60–100 км/ч: 8,9 с
% Аналог на ДВС: Honda Civic 1.5 (2016) — 7,6/5,7/6,7 л/100км
% 
% Honda Jazz 1.5 i-MMD (2020–2023)
% Расход топлива: 3,1/4,7/7,2 л/100км
% Смешанный расход (EcoTest): 5,1 л/100 км
% Разгон 60–100 км/ч: 6,4 с
% Аналог на ДВС: Honda Jazz 1.5 (2018) — 5,8/5,5/7,3 л/100км
% 
% Мягкие гибриды (Mild Hybrid, MHEV)
% 
% Honda Insight II 1.3 (2012)
% Расход топлива: 4,4/3,9/4,6 л/100км
% Разгон 60–100 км/ч: 8,2 с
% Аналог на ДВС: Honda Jazz 1.3 i-VTEC (2017) — 6,0/5,0/5,6 л/100км
% 
% VW Golf 1.5 eTSI (2024)
% Расход топлива: 5,5/5,1/5,7 л/100км
% Разгон 60–100 км/ч: 6,6 с
% Аналог на ДВС: VW Golf 1.5 TSI (2024) — 6,1/5,0/5,8 л/100км
% 
% Audi A5 2.0 TDI MHEV (2025)
% Расход топлива: 5,0/5,4/5,8 л/100км
% Разгон 60–100 км/ч: 4,3 с
% Аналог на ДВС: Audi A5 2.0 TFSI (2017) — 8,1/6,2/7,2 л/100км

% =========================
% Архитектуры и названия моделей
% =========================
archNames = { ...
    'Комбинированная схема (HSD)', ...
    'Параллельная схема (Inline)', ...
    'Многорежимная (MM-Series)', ...
    'Мягкая параллельная (Parallel MHEV)'};

modelNames = { ...
    {'Prius','Camry H','Fusion H'}, ...
    {'BMW 530d H','Sonata H','Elantra H'}, ...
    {'Accord H','Civic H','Jazz i-MMD'}, ...
    {'Insight','Golf eTSI','Audi A5 MHEV'}};

roadNames = {'Город','Шоссе'};

% =========================
% Цвета для графиков — темные, контрастные
% =========================
colors = [
    0.00 0.35 0.60;   % темно-синий
    0.70 0.25 0.05;   % темно-оранжевый
    0.40 0.10 0.45;   % темно-фиолетовый
    0.15 0.45 0.20;   % темно-зеленый
];

markers = {'o','s','d','^'};

% =========================
% Данные по расходу топлива
% Столбцы: [Город / Шоссе / Комбинированный]
% =========================
HSD.hybrid  = [4.4 4.7 4.5; 4.6 4.4 4.5; 5.5 5.7 5.6];
HSD.analog  = [7.8 6.2 7.1; 8.4 6.0 7.3; 11.2 7.6 9.4];

PAR.hybrid  = [6.2 4.9 5.6; 5.2 4.6 5.0; 4.8 4.5 4.7];
PAR.analog  = [7.0 5.4 6.3; 10.2 7.3 9.0; 9.0 7.1 8.1];

SER.hybrid  = [4.9 5.0 4.9; 5.5 5.2 5.3; 3.1 4.7 7.2];
SER.analog  = [10.2 6.9 8.7; 7.6 5.7 6.7; 5.8 5.5 7.3];

MILD.hybrid = [4.4 3.9 4.6; 5.5 5.1 5.7; 5.0 5.4 5.8];
MILD.analog = [6.0 5.0 5.6; 6.1 5.0 5.8; 8.1 6.2 7.2];

architectures = {HSD, PAR, SER, MILD};

% =========================
% Фигура 1 — Paired-slope графики
% =========================

figure('Color','w','Position',[50 50 1500 600]);

% Верхний ряд — Город
for a = 1:4
    subplot(2,4,a)
    hold on
    d = architectures{a};
    for m = 1:3
        plot([1 2], [d.analog(m,1), d.hybrid(m,1)], ...
            '-o','LineWidth',1.6,'Color',colors(a,:));
    end
    xticks([1 2])
    xticklabels({'ДВС аналог','Гибрид'})
    ylabel('Расход топлива, л/100 км')
    title(archNames{a})
    grid on
    xlim([0.9 2.2])
end

% Нижний ряд — Шоссе
for a = 1:4
    subplot(2,4,a+4)
    hold on
    d = architectures{a};
    for m = 1:3
        plot([1 2], [d.analog(m,2), d.hybrid(m,2)], ...
            '-o','LineWidth',1.6,'Color',colors(a,:));
    end
    xticks([1 2])
    xticklabels({'ДВС аналог','Гибрид'})
    ylabel('Расход топлива, л/100 км')
    title(archNames{a})
    grid on
    xlim([0.9 2.2])
end

% =========================
% Фигура 2 — Средний прирост (Bar plot)
% =========================
avgGain = zeros(4,2);

for a = 1:4
    d = architectures{a};
    g = (d.analog(:,1:2) - d.hybrid(:,1:2)) ./ d.analog(:,1:2);
    avgGain(a,:) = mean(g)*100;
end

figure('Color','w','Position',[300 200 800 600]);

% Город
subplot(2,1,1)
b1 = bar(avgGain(:,1));
b1.FaceColor = 'flat';
b1.EdgeColor = 'none';
b1.CData = colors;
ylabel('Снижение расхода, %')
title('Средний рост по архитектурам — Город')
xticks(1:4)
xticklabels(archNames)
grid on
ylim([0, 55])

% Численные значения над каждым баром — большой шрифт
for a = 1:4
    text(a, avgGain(a,1)+0.5, sprintf('%.1f%%', avgGain(a,1)), ...
        'HorizontalAlignment','center','VerticalAlignment', ...
        'bottom','FontSize',14,'FontWeight','bold', ...
		'Color',[0.1 0.1 0.1])
end

% Шоссе
subplot(2,1,2)
b2 = bar(avgGain(:,2));
b2.FaceColor = 'flat';
b2.EdgeColor = 'none';
b2.CData = colors;
ylabel('Снижение расхода, %')
title('Средний рост по архитектурам — Шоссе')
xticks(1:4)
xticklabels(archNames)
grid on
ylim([0, 35])

% Численные значения над каждым баром — большой шрифт
for a = 1:4
    text(a, avgGain(a,2)+0.5, sprintf('%.1f%%', avgGain(a,2)), ...
        'HorizontalAlignment','center','VerticalAlignment', ...
        'bottom','FontSize',14,'FontWeight','bold', ...
		'Color',[0.1 0.1 0.1])
end

% =========================
% Фигура 3 — Scatter график (эффективность vs динамика)
% =========================
accelHybrid = { ...
    [7.4, 4.9, 5.2], ...
    [3.2, 5.4, 8.1], ...
    [5.5, 8.9, 6.4], ...
    [8.2, 6.6, 4.3]};

figure('Color','w','Position',[400 250 950 550]);
hold on

xAll = [];
yAll = [];

for a = 1:4
    x = accelHybrid{a};
    d = architectures{a};
    y = (d.analog(:,3) - d.hybrid(:,3)) ./ d.analog(:,3) * 100;

    xAll = [xAll, x];
    yAll = [yAll; y];

    scatter(x, y, 140, ...
        'Marker', markers{a}, ...
        'MarkerFaceColor', colors(a,:), ...
        'MarkerEdgeColor','k','LineWidth',0.8);

    for m = 1:3
        dx = 0.10;
        ha = 'left';

        % Сдвиг текста слева для Sonata, Elantra, Jazz
        if ismember(modelNames{a}{m}, ...
                {'Sonata H','Elantra H','Jazz i-MMD'})
            dx = -0.10;
            ha = 'right';
        end

        text(x(m)+dx, y(m), modelNames{a}{m}, ...
            'FontSize',9, ...
            'Color',colors(a,:), ...
            'HorizontalAlignment',ha, ...
            'VerticalAlignment','middle');
    end
end

xlim([min(xAll)-0.6, max(xAll)+1.2])
ylim([min(yAll)-3,  max(yAll)+4])

xlabel('Время разгона 60–100 км/ч, с')
ylabel('Снижение расхода в комбинированном режиме, %')
title('Эффективность и динамика','FontWeight','bold')
legend(archNames,'Location','best')
grid on
