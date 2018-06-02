% You can use this skeleton or write your own.
% You are __STRONGLY__ suggest to run this script section-by-section using Ctrl+Enter.
% See http://www.mathworks.cn/cn/help/matlab/matlab_prog/run-sections-of-programs.html for more details.

%% Part1: Preceptron
range = [-1, 1];
nRep = 1000; % number of replicates
nTrain = 100; % number of training data
nTest = 100; % number of test data
avgIter = 0; % avage number of iteration
E_train = 0;
E_test = 0;
for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain);
    [w_g, iter] = perceptron(X, y);
    avgIter = avgIter + iter;
    E_train = E_train + sum(sum(w_g .* [ones(1, nTrain); X], 1).* y < 0);
    X_test = [ones(1, nTest); rand(2, nTest)*(range(2)-range(1)) + range(1)];
    y_test = sign(w_f'* X_test);
    E_test = E_test + sum(sum(w_g .* X_test, 1) .* y_test < 0);
end
E_train = E_train / nRep / nTrain;
E_test = E_test / nRep / nTest;
avgIter = avgIter / nRep;
fprintf('E_train is %f, E_test is %f.\n', E_train, E_test);
fprintf('Average number of iterations is %d.\n', avgIter);
plotdata(X, y, w_f, w_g, 'Pecertron');

%% Part2: Preceptron: Non-linearly separable case
nTrain = 100; % number of training data
[X, y, w_f] = mkdata(nTrain, 'noisy');
[w_g, iter] = perceptron(X, y);

%% Part3: Linear Regression
nRep = 1000; % number of replicates
nTrain = 100; % number of training data
nTest = 100; % number of test data
E_train = 0;
E_test = 0;
for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain);
    w_g = linear_regression(X, y);
    E_train = E_train + sum(sum(w_g .* [ones(1, nTrain); X], 1).* y < 0);
    X_test = [ones(1, nTest); rand(2, nTest)*(range(2)-range(1)) + range(1)];
    y_test = sign(w_f'* X_test);
    E_test = E_test + sum(sum(w_g .* X_test, 1) .* y_test < 0);
end
E_train = E_train / nRep / nTrain;
E_test = E_test / nRep / nTest;
fprintf('E_train is %f, E_test is %f.\n', E_train, E_test);
plotdata(X, y, w_f, w_g, 'Linear Regression');

%% Part4: Linear Regression: noisy
nRep = 1000; % number of replicates
nTrain = 100; % number of training data
nTest = 100; % number of test data
E_train = 0;
E_test = 0;
for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain, 'noisy');
    w_g = linear_regression(X, y);
    E_train = E_train + sum(sum(w_g .* [ones(1, nTrain); X], 1).* y < 0);
    X_test = [ones(1, nTest); rand(2, nTest)*(range(2)-range(1)) + range(1)];
    y_test = sign(w_f'* X_test);
    E_test = E_test + sum(sum(w_g .* X_test, 1) .* y_test < 0);
end
E_train = E_train / nRep / nTrain;
E_test = E_test / nRep / nTest;
fprintf('E_train is %f, E_test is %f.\n', E_train, E_test);
plotdata(X, y, w_f, w_g, 'Linear Regression: noisy');

%% Part5: Linear Regression: poly_fit
load('poly_train', 'X', 'y');
load('poly_test', 'X_test', 'y_test');
w = linear_regression(X, y);
E_train = sum(sum(w .* [ones(1, size(X, 2)); X], 1).* y < 0);
E_train = E_train / size(X, 2);
E_test = sum(sum(w .* [ones(1, size(X_test, 2)); X_test], 1) .* y_test < 0);
E_test = E_test / size(X_test,2);
fprintf('E_train is %f, E_test is %f.\n', E_train, E_test);

% poly_fit with transform
X_t = [X; X(1,:) .* X(2,:); X(1,:).^2; X(2,:).^2]; % CHANGE THIS LINE TO DO TRANSFORMATION
X_test_t = [X_test; X_test(1, :) .* X_test(2, :); X_test(1, :).^2; X_test(2, :).^2]; % CHANGE THIS LINE TO DO TRANSFORMATION
w = linear_regression(X_t, y);
E_train = sum(sum(w .* [ones(1, size(X_t, 2)); X_t], 1).* y < 0);
E_train = E_train / size(X_t, 2);
E_test = sum(sum(w .* [ones(1, size(X_test_t, 2)); X_test_t], 1) .* y_test < 0);
E_test = E_test / size(X_test_t,2);
fprintf('E_train is %f, E_test is %f.\n', E_train, E_test);


%% Part6: Logistic Regression
nRep = 100; % number of replicates
nTrain = 100; % number of training data
nTest = 100; % number of test data
E_train = 0;
E_test = 0;
for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain);
    w_g = logistic(X, y);
    E_train = E_train + sum(sum(w_g .* [ones(1, nTrain); X], 1).* y < 0);
    X_test = [ones(1, nTest); rand(2, nTest)*(range(2)-range(1)) + range(1)];
    y_test = sign(w_f'* X_test);
    E_test = E_test + sum(sum(w_g .* X_test, 1) .* y_test < 0);
end
E_train = E_train / nRep / nTrain;
E_test = E_test / nRep / nTest;
fprintf('E_train is %f, E_test is %f.\n', E_train, E_test);
plotdata(X, y, w_f, w_g, 'Logistic Regression');

%% Part7: Logistic Regression: noisy
nRep = 100; % number of replicates
nTrain = 100; % number of training data
nTest = 100; % number of training data
E_train = 0;
E_test = 0;
for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain, 'noisy');
    w_g = logistic(X, y);
    E_train = E_train + sum(sum(w_g .* [ones(1, nTrain); X], 1).* y < 0);
    X_test = [ones(1, nTest); rand(2, nTest)*(range(2)-range(1)) + range(1)];
    y_test = sign(w_f'* X_test);
    E_test = E_test + sum(sum(w_g .* X_test, 1) .* y_test < 0);
end
E_train = E_train / nRep / nTrain;
E_test = E_test / nRep / nTest;
fprintf('E_train is %f, E_test is %f.\n', E_train, E_test);
plotdata(X, y, w_f, w_g, 'Logistic Regression: noisy');

%% Part8: SVM
nRep = 1000; % number of replicates
nTrain = 30; % number of training data
nTest = 100; % number of training data
num_sv = 0;
E_train = 0;
E_test = 0;
for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain);
    [w_g, num_sc] = svm(X, y);
    num_sv = num_sv + num_sc;
    E_train = E_train + sum(sum(w_g .* [ones(1, nTrain); X], 1).* y < 0);
    X_test = [ones(1, nTest); rand(2, nTest)*(range(2)-range(1)) + range(1)];
    y_test = sign(w_f'* X_test);
    E_test = E_test + sum(sum(w_g .* X_test, 1) .* y_test < 0);
end
E_train = E_train / nRep / nTrain;
E_test = E_test / nRep / nTest;
num_sv = E_train / nRep;
fprintf('E_train is %f, E_test is %f.\n', E_train, E_test);
fprintf('Average number of supported vectors is %d.\n', num_sv);
plotdata(X, y, w_f, w_g, 'SVM');
