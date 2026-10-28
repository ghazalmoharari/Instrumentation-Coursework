clear; clc; close all;
data = readtable('SensorDataQ2.csv');
X= data.x;
V_inc = data.y;
V_dec=data.z;
%% PART (A): ِDATA PLOT
figure;
plot(X, V_inc, 'r-o', 'LineWidth', 1.5, 'DisplayName', 'Increasing'); hold on;
plot(X, V_dec, 'b-s', 'LineWidth', 1.5, 'DisplayName', 'Decreasing');
grid on;
xlabel('Displacement X (mm)');
ylabel('Voltage V (Volt)');
title('Sensor Calibration: Hysteresis and Dead Zone');
legend;

%% PART (B): CALCULATE HYSTERSIS ERROR
eh = abs(V_inc - V_dec);
FS = max(V_dec) - min(V_dec); 
[max_error, idx] = max(eh);
max_error_percentage = (max_error / FS) * 100;

fprintf('------------------------------------------------------------\n');
fprintf('%-10s | %-10s | %-10s | %-15s\n', 'X (mm)', 'V_inc (V)', 'V_dec (V)', 'Hyst. Error (V)');
fprintf('------------------------------------------------------------\n');

for i = 1:length(X)
    fprintf('%-10.1f | %-10.2f | %-10.2f | %-15.2f\n', X(i), V_inc(i), V_dec(i), eh(i));
end

fprintf('------------------------------------------------------------\n');
fprintf('Summary of Results:\n');
fprintf('Maximum Hysteresis Error: %.2f Volts (at X = %.1f mm)\n', max_error, X(idx));
fprintf('Full Scale (FS): %.2f Volts\n', FS);
fprintf('Maximum Hysteresis Error Calculation: (%.2f / %.2f) * 100\n', max_error, FS);
fprintf('Maximum Hysteresis Error: %.2f%% of Full Scale\n', max_error_percentage);
fprintf('------------------------------------------------------------\n');