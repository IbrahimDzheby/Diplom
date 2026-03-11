clear
clc
close all

%% Загрузка данных
M = readmatrix('BSFC.xlsx');
rpm    = M(:,1);
torque = M(:,2);
bsfc   = M(:,3);

uniqueLevels = unique(bsfc);

%% Сортировка точек на каждом контуре по углу обхода
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
    
    contours{i} = [x(order), y(order), repmat(level,numel(x),1)];
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
title('BSFC ведро: ровная поверхность без щелей и пронизываний')

colors = hot(length(uniqueLevels));
FaceAlphaVal = 0.7;

%% Соединение соседних уровней
for i = 1:length(contours)-1
    lower = contours{i};
    upper = contours{i+1};
    
    if isempty(lower) || isempty(upper)
        continue
    end
    
    nLower = size(lower,1);
    
    % Интерполируем верхний контур по длине нижнего
    % Кумулятивная длина нижнего
    sLower = [0; cumsum(sqrt(sum(diff(lower(:,1:2)).^2,2)))];
    sLower = sLower / sLower(end);
    
    % Кумулятивная длина верхнего
    sUpper = [0; cumsum(sqrt(sum(diff(upper(:,1:2)).^2,2)))];
    sUpper = sUpper / sUpper(end);
    
    % Интерполируем верхний контур ровно по nLower точкам
    upperX = interp1(sUpper, upper(:,1), sLower,'linear','extrap');
    upperY = interp1(sUpper, upper(:,2), sLower,'linear','extrap');
    upperZ = repmat(upper(1,3), nLower,1);
    
    c = (colors(i,:) + colors(i+1,:))/2;
    
    % Два треугольника на каждую пару соседних точек нижнего контура
    for j = 1:nLower
        jNext = mod(j,nLower)+1;
        
        p1 = lower(j,:); p2 = lower(jNext,:);
        q1 = [upperX(j), upperY(j), upperZ(j)];
        q2 = [upperX(jNext), upperY(jNext), upperZ(jNext)];
        
        fill3([p1(1),p2(1),q1(1)], [p1(2),p2(2),q1(2)], [p1(3),p2(3),q1(3)], ...
            c,'FaceAlpha',FaceAlphaVal,'EdgeColor','k','LineWidth',0.5)
        fill3([q1(1),p2(1),q2(1)], [q1(2),p2(2),q2(2)], [q1(3),p2(3),q2(3)], ...
            c,'FaceAlpha',FaceAlphaVal,'EdgeColor','k','LineWidth',0.5)
    end
end

colormap(hot)
colorbar