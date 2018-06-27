function idx = spectral(W, k)
%SPECTRUAL spectral clustering
%   Input:
%     W: Adjacency matrix, N-by-N matrix
%     k: number of clusters
%   Output:
%     idx: data point cluster labels, n-by-1 vector.

% YOUR CODE HERE
D = zeros(size(W));
D(logical(eye(size(W)))) = sum(W,2);
[Y, ~] = eig(D - W, D);
idx = kmeans(Y(:,2), k);
end
