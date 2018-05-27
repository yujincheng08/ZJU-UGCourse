%clear all; close all; clc;

%%
%%%%%%%%%%%%%%% load image %%%%%%%%%%%%%%%%%%%%%%%%%
testName = 'B';
testName = ['../../img/', testName];
corrImg = im2double(imread([testName '.png']));

%%%%%%%%%%%%%%% generate corrupted image %%%%%%%%%%%
[rows, cols, channels] = size(corrImg);
noiseRatio = 0.6; % A: 0.8, B: 0.4, C: 0.6
% noiseMask = ones(rows, cols, channels);
% subNoiseNum = round(noiseRatio * cols);
% for k = 1:channels
%     for i = 1: rows
%         tmp = randperm(cols);
%         noiseIdx = tmp(1:subNoiseNum);
%         noiseMask(i, noiseIdx, k) = zeros(1, subNoiseNum);
%     end
% end
% corrImg = Img .* noiseMask;
% imwrite(corrImg, [testName, '.png'], 'png');

noiseMask = double(corrImg~=0);

% standardize the corrupted image
minX = min(min(min(corrImg)));
maxX = max(max(max(corrImg)));
corrImg = (corrImg - minX)/(maxX-minX);
% noiseMask = ceil(corrImg);

%% ==========================learn the coefficents in regression function=================
% In this section, we use gaussian kernels as the basis functions. And we
% do regression analysis each row at a time.

basisNum = 50; % define the number of basis functions.
sigma = 0.01; % define the standard deviation.
Phi_mu = linspace(1, cols, basisNum)/cols; % set the mean value of each basis function
Phi_sigma = sigma * ones(1, basisNum); % here we set the standard deviation to the same value for brevity.

% use pixel index as the independent variable in the regression function
x = 1:cols;
x = (x - min(x)) / (max(x)-min(x));

% select the missing pixels randomly
resImg = corrImg;

% for each channel
for k = 1:channels
    % for each row
    for i = 1: rows
        % select the missing pixels each row
        msk = noiseMask(i, :, k);
        msk = msk(:);
        misIdx = find(msk<1);
        misNum = length(misIdx);
        ddIdx = find(msk>=1);
        ddNum = length(ddIdx);
        
        % compute the coefficients
        Phi = [ones(ddNum, 1) zeros(ddNum, basisNum-1)];
        for j = 2: basisNum
            Phi(:, j) = normpdf(x(ddIdx)', Phi_mu(j-1), Phi_sigma(j-1)) * sqrt(2*pi) * Phi_sigma(j-1);
        end
        co = corrImg(i,ddIdx,k);
        w = pinv(Phi' * Phi) * Phi' * corrImg(i,ddIdx,k)';
        
        % restore the missing values
        Phi1 = [ones(misNum, 1) zeros(misNum, basisNum-1)];
        for j = 2: basisNum
            Phi1(:, j) = normpdf(x(misIdx)', Phi_mu(j-1), Phi_sigma(j-1)) * sqrt(2*pi) * Phi_sigma(j-1);
        end
        resImg(i,misIdx,k) = w' * Phi1';
    end
end
resImg = min(resImg, 1);
resImg = max(resImg, 0);
%% show the restored image
figure;
imshow(corrImg);
figure;
imshow(resImg);

% store figure
prefix = [testName, '_', num2str(basisNum), '_', num2str(noiseRatio)];
imwrite(resImg, [prefix, '.png'], 'png');
%%
% fprintf(['Distance between original and corrupted: ' num2str(norm(Img(:)-corrImg(:), 2)) '\n']);
% fprintf(['Distance between original and reconstructed (regression): ' num2str(norm(Img(:)-resImg(:), 2)) '\n']);
