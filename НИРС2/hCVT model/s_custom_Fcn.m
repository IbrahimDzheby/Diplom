function s = s_custom_Fcn(SOC)
    % Кусочно-линейная интерполяция для s(SOC) (сгенерировано скриптом)
    SOC_points = [0.00, 0.30, 0.50, 0.60, 1.00];
    s_points   = [0.11, 0.10, 0.07, 0.02, 0.01];
    s = interp1(SOC_points, s_points, SOC, 'linear', 'extrap');
    s = min(max(s, 0.02), 0.15); % Защита от выхода за границы
end
