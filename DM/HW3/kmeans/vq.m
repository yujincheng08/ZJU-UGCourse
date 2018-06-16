img = imread('sample.jpg');
fea = double(reshape(img, size(img, 1)*size(img, 2), 3));
% YOUR (TWO LINE) CODE HERE
% [idx, ctrs, ~] = Kmeans(fea, 64);
% fea = ctrs(idx, :);
imshow(uint8(reshape(fea, size(img))));
