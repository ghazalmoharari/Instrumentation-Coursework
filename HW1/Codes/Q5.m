clear; clc; close all;
%% PART (A): ِDATA PLOT
data = readmatrix('Data.csv'); 
data(:,2) = 0.9*ones(height(data), 1); 
t = 0:0.1:5;

y1 = data(:,1); 
y2 = data(:,2); 
y3 = data(:,3); 

figure;
subplot(3,1,1); plot(t, y1, 'r', 'LineWidth', 1.5); title('Sensor 1 Output'); grid on;
subplot(3,1,2); plot(t, y2, 'b', 'LineWidth', 1.5); title('Sensor 2 Output'); grid on;
subplot(3,1,3); plot(t, y3, 'g', 'LineWidth', 1.5); title('Sensor 3 Output'); grid on;
xlabel('Time (sec)');
