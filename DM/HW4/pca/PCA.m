function [eigvector, eigvalue] = PCA(data)
%PCA	Principal Component Analysis
%
%             Input:
%               data       - Data matrix. Each row vector of fea is a data point.
%
%             Output:
%               eigvector - Each column is an embedding function, for a new
%                           data point (row vector) x,  y = x*eigvector
%                           will be the embedding result of x.
%               eigvalue  - The sorted eigvalue of PCA eigen-problem.
%

% YOUR CODE HERE
S = cov(data);
[eigvector, eigvalue] = eig(S, 'vector');
[eigvalue, I] = sort(eigvalue, 1, 'descend');
eigvector = eigvector(:, I);
end