load('digit_data', 'X');
Ks = [10, 20, 50];
for i=1:size(Ks,2)
    K = Ks(i);
    [idx, ctrs, iter_ctrs] = Kmeans(X, K);
    figure;show_digit(ctrs);title(K);
end
