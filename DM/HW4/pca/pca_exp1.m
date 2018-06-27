load('ORL_data', 'fea_Train', 'gnd_Train', 'fea_Test', 'gnd_Test');
% YOUR CODE HERE

% 1. Feature preprocessing

% 2. Run PCA
[eigvector, ~] = PCA(fea_Train);
% 3. Visualize eigenface
show_face(eigvector');
% 4. Project data on to low dimensional space
dimension = 128;
transformation = eigvector(:, 1:dimension);
low_Train = fea_Train * transformation;
low_Test = fea_Test * transformation;
% 5. Run KNN in low dimensional space
gnd = knn(low_Test', low_Train', gnd_Train', 1);
acc = sum(gnd'==gnd_Test) / length(gnd_Test);
disp(['accuray with dimension ', num2str(dimension), ' is ', num2str(acc)]);
% 6. Recover face images form low dimensional space, visualize them
fea = low_Test * transformation';
figure; show_face(fea);title(dimension)