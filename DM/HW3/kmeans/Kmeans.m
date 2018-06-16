function [idx, ctrs, iter_ctrs] = Kmeans(X, K)
%KMEANS K-Means clustering algorithm
%
%   Input: X - data point features, n-by-p maxtirx.
%          K - the number of clusters
%
%   OUTPUT: idx  - cluster label
%           ctrs - cluster centers, K-by-p matrix.
%           iter_ctrs - cluster centers of each iteration, K-by-p-by-iter
%                       3D matrix.
ctrs = datasample(X, K, 1, 'Replace', false);
idx = zeros(1, size(X,1));
iter_ctrs = ctrs;
while(true)
    [~, idx_new] = pdist2(ctrs, X, 'euclidean','Smallest', 1);
    if(idx_new == idx)
        break;
    end
    for i=1:K
        ctrs(i,:) = mean(X(idx_new == i, :));
    end
    iter_ctrs = cat(3, iter_ctrs, ctrs);
    idx = idx_new;
end
end
