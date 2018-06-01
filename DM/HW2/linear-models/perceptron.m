function [w, iter] = perceptron(X, y)
%PERCEPTRON Perceptron Learning Algorithm.
%
%   INPUT:  X: training sample features, P-by-N matrix.
%           y: training sample labels, 1-by-N row vector.
%
%   OUTPUT: w:    learned perceptron parameters, (P+1)-by-1 column vector.
%           iter: number of iterations
%

% YOUR CODE HERE
w = [1; rand(size(X, 1),1)];
flag = 0;
iter = 0;
while(flag == 0 && iter < 100000)
    iter = iter + 1;
    flag = 1;
    for i = 1:size(X,2)
        x = [1; X(:, i)];
        if(w' * x * y(i) < 0)
            w = w + x * y(i);
            flag = 0;
        end
    end
end
end
