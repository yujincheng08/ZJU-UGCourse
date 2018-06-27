function img = hack_pca(filename)
% Input: filename -- input image file name/path
% Output: img -- image without rotation

img_r = double(imread(filename));
% YOUR CODE HERE
[x, y] = find(img_r<255);
[eigvector, ~] = PCA([x y]);
idx = round([x y] * eigvector);
idx(:,2) = -idx(:,2);
idx = idx - min(idx) + 1;
img = ones(max(idx(:,2)), max(idx(:,1))) * 255;
img(sub2ind(size(img), idx(:,2), idx(:,1))) = img_r(sub2ind(size(img_r), x, y));
img = uint8(img);
imshow(img);
end