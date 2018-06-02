%% Ridge Regression
load('digit_train', 'X', 'y');
X = (X - mean(X)) ./ std(X);
% Do feature normalization
% ...

% Do LOOCV
lambdas = [1e-3, 1e-2, 1e-1, 0, 1, 1e1, 1e2, 1e3];
E = zeros(size(lambdas));
ws = zeros(size(X, 1) + 1, size(lambdas, 2));
for i = 1:length(lambdas)
    E_val = 0;
    for j = 1:size(X, 2)
        X_ = X(:, [1:j-1,j+1:end]); y_ = y([1:j-1, j+1:end]); % take point j out of X
        w = ridge(X_, y_, lambdas(i));
        E_val = E_val + (w' * [1; X(:,j)] * y(j) < 0);
    end
    E(i) = E_val;
    ws(:,i) = w;
end

% Compute training error

load('digit_test', 'X_test', 'y_test');
[~, best_idx] = min(E);
% Do feature normalization
X_test = (X_test - mean(X_test)) ./ std(X_test);
% Compute test error
E_train = sum(sum(ws(:, best_idx) .* [ones(1, size(X, 2)); X], 1).* y < 0) / size(X, 2);
E_test = sum(sum(ws(:, best_idx) .* [ones(1, size(X_test, 2)); X_test], 1).* y_test < 0) / size(X_test, 2);
lambda = lambdas(best_idx);
fprintf('lambda is %f, E_train is %f, E_test is %f.\n', lambda, E_train, E_test);
zero_idx = find(lambdas == 0);
E_train = sum(sum(ws(:, zero_idx) .* [ones(1, size(X, 2)); X], 1).* y < 0) / size(X, 2);
E_test = sum(sum(ws(:, zero_idx) .* [ones(1, size(X_test, 2)); X_test], 1).* y_test < 0) / size(X_test, 2);
fprintf('Without regularization, E_train is %f, E_test is %f.\n', E_train, E_test);



%% Logistic
load('digit_train', 'X', 'y');
X = (X - mean(X)) ./ std(X);
% Do feature normalization
% ...

% Do LOOCV
lambdas = [1e-3, 1e-2, 1e-1, 0, 1, 1e1, 1e2, 1e3];
E = zeros(size(lambdas));
ws = zeros(size(X, 1) + 1, size(lambdas, 2));
for i = 1:length(lambdas)
    E_val = 0;
    for j = 1:size(X, 2)
        X_ = X(:, [1:j-1,j+1:end]); y_ = y([1:j-1, j+1:end]); % take point j out of X
        w = logistic_r(X_, y_, lambdas(i));
        E_val = E_val + (w' * [1; X(:,j)] * y(j) < 0);
    end
    E(i) = E_val;
    ws(:,i) = w;
end

% Compute training error

load('digit_test', 'X_test', 'y_test');
[~, best_idx] = min(E);
% Do feature normalization
X_test = (X_test - mean(X_test)) ./ std(X_test);
% Compute test error
E_train = sum(sum(ws(:, best_idx) .* [ones(1, size(X, 2)); X], 1).* y < 0) / size(X, 2);
E_test = sum(sum(ws(:, best_idx) .* [ones(1, size(X_test, 2)); X_test], 1).* y_test < 0) / size(X_test, 2);
lambda = lambdas(best_idx);
fprintf('lambda is %f, E_train is %f, E_test is %f.\n', lambda, E_train, E_test);
zero_idx = find(lambdas == 0);
E_train = sum(sum(ws(:, zero_idx) .* [ones(1, size(X, 2)); X], 1).* y < 0) / size(X, 2);
E_test = sum(sum(ws(:, zero_idx) .* [ones(1, size(X_test, 2)); X_test], 1).* y_test < 0) / size(X_test, 2);
fprintf('Without regularization, E_train is %f, E_test is %f.\n', E_train, E_test);

%% SVM with slack variable
