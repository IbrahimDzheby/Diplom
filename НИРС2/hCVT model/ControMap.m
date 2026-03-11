clear
clc
close all

%% Загрузка данных
M = readmatrix('BSFC.xlsx');
rpm    = M(:,1);
torque = M(:,2);
bsfc   = M(:,3);

uniqueLevels = unique(bsfc);
nInterp = 200; % количество точек после интерполяции для каждого контура

%% Сортировка и интерполяция каждого контура по длине
contours = cell(length(uniqueLevels),1);
for i = 1:length(uniqueLevels)
    level = uniqueLevels(i);
    idx = bsfc == level;
    x = rpm(idx);
    y = torque(idx);
    
    if numel(x) < 2
        continue
    end
    
    % Центр линии для порядка обхода
    xc = mean(x); yc = mean(y);
    angles = atan2(y - yc, x - xc);
    [~, order] = sort(angles);
    x = x(order); y = y(order);
    
    % Кумулятивная длина
    s = [0; cumsum(sqrt(diff(x).^2 + diff(y).^2))];
    s = s / s(end);
    
    % Интерполяция контура на nInterp точек
    sInterp = linspace(0,1,nInterp);
    xInterp = interp1(s,x,sInterp,'linear','extrap');
    yInterp = interp1(s,y,sInterp,'linear','extrap');
    
    contours{i} = [xInterp', yInterp', repmat(level,nInterp,1)];
end

%% Построение
figure
hold on
axis tight
grid on
view(45,30)
camlight
lighting gouraud
xlabel('n (rpm)')
ylabel('T (Nm)')
zlabel('BSFC')
title('BSFC ведро: яркие треугольники без лишних нитей')

% Яркая цветовая карта
colors = hot(length(uniqueLevels)); % можно parula, autumn, jet

%% Треугольники между соседними уровнями
for i = 1:length(contours)-1
    lower = contours{i};
    upper = contours{i+1};
    
    if isempty(lower) || isempty(upper)
        continue
    end
    
    n = size(lower,1);
    
    % Цвет среднего уровня (яркий)
    c = (colors(i,:) + colors(i+1,:))/2;
    
    for j = 1:n
        jNext = mod(j,n)+1;
        p1 = lower(j,:); p2 = lower(jNext,:);
        q1 = upper(j,:); q2 = upper(jNext,:);
        
        fill3([p1(1),p2(1),q1(1)], [p1(2),p2(2),q1(2)], [p1(3),p2(3),q1(3)], c, ...
            'FaceAlpha',0.7, 'EdgeColor','k', 'LineWidth',0.5)
        fill3([q1(1),p2(1),q2(1)], [q1(2),p2(2),q2(2)], [q1(3),p2(3),q2(3)], c, ...
            'FaceAlpha',0.7, 'EdgeColor','k', 'LineWidth',0.5)
    end
end

colormap(hot)
colorbar