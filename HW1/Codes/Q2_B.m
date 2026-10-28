clear; clc; close all;
%% Find Best LS Degree
train_data = readtable('data_B_train.csv');
test_data = readtable('data_B_test.csv');

x_train = train_data.x;
y_train = train_data.y;
x_test = test_data.x;
y_test = test_data.y;

max_degree = 10;
train_errors = zeros(max_degree, 1);
test_errors = zeros(max_degree, 1);
degrees = 1:max_degree;

function Phi = create_polynomial_features(x, degree)
    Phi = ones(length(x), 1);
    for d = 1:degree
        Phi = [Phi, x.^d];
    end
end

for degree = degrees
    Phi_train = create_polynomial_features(x_train, degree);
    Phi_test = create_polynomial_features(x_test, degree);
    
    theta = (Phi_train' * Phi_train) \ (Phi_train' * y_train);
    
    y_train_pred = Phi_train * theta;
    y_test_pred = Phi_test * theta;
    
    train_errors(degree) = mean((y_train - y_train_pred).^2);
    test_errors(degree) = mean((y_test - y_test_pred).^2);
end

[min_test_error, best_degree] = min(test_errors);

fprintf('Best degree based on test error: %d\n', best_degree);
fprintf('Best degree training error: %.6f\n', train_errors(best_degree));
fprintf('Best degree testing error: %.6f\n', min_test_error);

% Plotting Training and Test Errors
figure;
plot(degrees, train_errors, 'b--', 'LineWidth', 2.5, 'MarkerSize', 10, 'MarkerFaceColor', 'b');
hold on;
plot(degrees, test_errors, 'r-.', 'LineWidth', 2.5, 'MarkerSize', 10, 'MarkerFaceColor', 'r');
xlabel('Polynomial Degree', 'FontSize', 14, 'FontWeight', 'bold');
ylabel('MSE', 'FontSize', 14, 'FontWeight', 'bold');
legend('Training Error', 'Test Error', 'Location', 'northeast', 'FontSize', 12);
title('Training and Test Error vs Polynomial Degree', 'FontSize', 14, 'FontWeight', 'bold');
grid on;
set(gca, 'FontSize', 12, 'GridLineStyle', '--', 'GridAlpha', 0.3);

% Best Model
Phi_train_best = create_polynomial_features(x_train, best_degree);
Phi_test_best = create_polynomial_features(x_test, best_degree);
theta_best = (Phi_train_best' * Phi_train_best) \ (Phi_train_best' * y_train);

% Best Model Plot
figure;
x_plot = linspace(min([x_train; x_test]), max([x_train; x_test]), 200)';
Phi_plot = create_polynomial_features(x_plot, best_degree);
y_plot = Phi_plot * theta_best;

scatter(x_train, y_train, 80, 'filled', 'MarkerEdgeColor', [0 0.447 0.741], ...
    'MarkerFaceColor', [0 0.447 0.741], 'LineWidth', 1.5, 'DisplayName', 'Training Data');
hold on;
scatter(x_test, y_test, 80, 'filled', 'MarkerEdgeColor', [0.85 0.325 0.098], ...
    'MarkerFaceColor', [0.85 0.325 0.098], 'LineWidth', 1.5, 'DisplayName', 'Test Data');
plot(x_plot, y_plot, 'k-', 'LineWidth', 3, 'DisplayName', sprintf('Polynomial Degree %d', best_degree));
xlabel('x', 'FontSize', 14, 'FontWeight', 'bold');
ylabel('y', 'FontSize', 14, 'FontWeight', 'bold');
title(sprintf('Best Polynomial Model (Degree %d)', best_degree), 'FontSize', 14, 'FontWeight', 'bold');
legend('Location', 'best', 'FontSize', 12);
grid on;
set(gca, 'FontSize', 12, 'GridLineStyle', '--', 'GridAlpha', 0.3);

% Display Error Table
fprintf('\nError Table:\n');
fprintf('Degree\tTraining Error\tTest Error\n');
for d = degrees
    fprintf('%d\t%.6f\t\t%.6f\n', d, train_errors(d), test_errors(d));
end