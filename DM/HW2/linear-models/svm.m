function [w, num] = svm(X, y)
%SVM Support vector machine.
%
%   INPUT:  X: training sample features, P-by-N matrix.
%           y: training sample labels, 1-by-N row vector.
%
%   OUTPUT: w:    learned perceptron parameters, (P+1)-by-1 column vector.
%           num:  number of support vectors
%

% YOUR CODE HERE
x = [ones(1, size(X,2)); X];
H = eye(size(x,1));
H(1,1) = 0;
w = quadprog(H, zeros(1, size(x,1)), -(y.*x)', -ones(1, size(x,2)));
num = sum(y .* (w' * x) <= 1.00001);
end
