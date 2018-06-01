function w = logistic(X, y)
%LR Logistic Regression.
%
%   INPUT:  X:   training sample features, P-by-N matrix.
%           y:   training sample labels, 1-by-N row vector.
%
%   OUTPUT: w    : learned parameters, (P+1)-by-1 column vector.
%

% YOUR CODE HERE

w = rand(size(X, 1) + 1, 1);
rate = 0.1;
for i=1:100
    x = [ones(1, size(X, 2)); X];
    w = w - rate * sum(-y ./ ( 1 + exp(w' * x .* y)) .* x,2);
end
end
