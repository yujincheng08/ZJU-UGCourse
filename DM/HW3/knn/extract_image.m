function X = extract_image(image_file_name)
%EXTRACT_IMAGE Extract features from image
%   Inputs:
%       image_file_name: filename of image
%   Outputs:
%       X: 140x5 matrix, 5 digits in an image, each digit is a 140x1 vector.

m = imread(image_file_name);
d1 = reshape(m(5:18, 5:14), 140, 1);
d2 = reshape(m(5:18, 14:23), 140, 1);
d3 = reshape(m(5:18, 23:32), 140, 1);
d4 = reshape(m(5:18, 32:41), 140, 1);
d5 = reshape(m(5:18, 41:50), 140, 1);
X = [d1, d2, d3, d4, d5];

end
