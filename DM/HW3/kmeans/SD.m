largest_dist=-inf;
smallest_dist=inf;
load('kmeans_data', 'X');
ctrs = [];
for x=1:1000
    [idx, ctrs_new, iter_ctrs] = Kmeans(X, 2);
    dist = 0;
    for i = 2:size(iter_ctrs, 3)
        dist = dist + (iter_ctrs(:,:,i-1) - iter_ctrs(:,:,i)).^2;
    end
    if(dist > largest_dist)
        largest_idx = idx;
        largest_ctrs = ctrs_new;
        largest_iter_ctrs = iter_ctrs;
        largest_dist = dist;
    end
    if(dist < smallest_dist)
        smallest_idx = idx;
        smallest_ctrs = ctrs_new;
        smallest_iter_ctrs = iter_ctrs;
        smallest_dist = dist;
    end
    ctrs = ctrs_new;

end
kmeans_plot(X, largest_idx, largest_ctrs, largest_iter_ctrs);
kmeans_plot(X, smallest_idx, smallest_ctrs, smallest_iter_ctrs);