
clear; clc; close all;

% 1. Load Data
data = readtable('data_A.csv');
x = data.x;
y = data.y;
[x_sorted, idx] = sort(x);
y_sorted = y(idx);

%% Part (a): LS for degrees 1, 3, and 9
degrees = [1, 3, 9];
figure('Name', 'Part (a) - LS Estimation');
plot(x_sorted, y_sorted, 'k.', 'MarkerSize', 8); hold on;
colors = ['r', 'g', 'b'];

for i = 1:length(degrees)
    n = degrees(i);
    phi = zeros(length(x), n+1);
    for p = 0:n
        phi(:, p+1) = x.^p;
    end

    theta_ls = (phi' * phi) \ (phi' * y);
    phi_plot = zeros(length(x_sorted), n+1);
    for p = 0:n
        phi_plot(:, p+1) = x_sorted.^p;
    end
    y_pred = phi_plot * theta_ls;
    
    plot(x_sorted, y_pred, colors(i), 'LineWidth', 1.5);
    fprintf('LS Degree %d coefficients calculated.\n', n);
end

legend('Original Data', 'Degree 1', 'Degree 3', 'Degree 9');
title('Polynomial Estimation using LS');
grid on;

%% Part (b): RLS for degree 9
n_rls = 9;
num_data = length(x);
num_params = n_rls + 1;

% Initialization
P = 10^5 * eye(num_params);
theta_rls = zeros(num_params, 1);

% RLS Algorithm
for k = 1:num_data
    % Constructing phi_k for current data point
    phi_k = zeros(num_params, 1);
        for p = 0:n_rls
        phi_k(p+1) = x(k)^p;
    end
    
    % K gain calculation
    K = (P * phi_k) / (1 + phi_k' * P * phi_k);
    
    % Update theta
    error = y(k) - phi_k' * theta_rls;
    theta_rls = theta_rls + K * error;
    
    % Update P matrix
    P = (eye(num_params) - K * phi_k') * P;
end

fprintf('\nRLS Degree 9 parameters obtained.\n');

%% Part (c): Comparison and Reporting
% Calculate LS for degree 9 again to compare
phi9 = zeros(length(x), 10);
for p = 0:9
    phi9(:, p+1) = x.^p;
end
theta_ls9 = (phi9' * phi9) \ (phi9' * y);

% Display comparison table
disp('Comparison of Parameters (Degree 9):');
ComparisonTable = table((0:9)', theta_ls9, theta_rls, ...
    'VariableNames', {'Power', 'LS_Theta', 'RLS_Theta'});
disp(ComparisonTable);

% Final Plot
phi9_plot = zeros(length(x_sorted), 10);
for p = 0:9
    phi9_plot(:, p+1) = x_sorted.^p;
end

figure('Name', 'Part (c) - LS vs RLS Comparison');
plot(x_sorted, y_sorted, 'k.', 'DisplayName', 'Original Data'); hold on;
plot(x_sorted, phi9_plot * theta_ls9, 'r--', 'LineWidth', 2, 'DisplayName', 'LS (Deg 9)');
plot(x_sorted, phi9_plot * theta_rls, 'b:', 'LineWidth', 2, 'DisplayName', 'RLS (Deg 9)');
legend;
title('Comparison of LS and RLS (Degree 9)');
grid on;