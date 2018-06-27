function W = knn_graph(X, k, threshold)
%KNN_GRAPH Construct W using KNN graph
%   Input: X - data point features, n-by-p maxtirx.
%          k - number of nn.
%          threshold - distance threshold.
%
%   Output:W - adjacency matrix, n-by-n matrix.

% YOUR CODE HERE
D = EuDist2(X);
W = ((D <= max(mink(D, k+1)))) & (D <= threshold) - eye(size(D));
W = double(W | W');
end
