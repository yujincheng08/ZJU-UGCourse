% mu: 2x1 matrix
% Sigma: 2x2 matrix
% phi: a number

% for test
% Sigma = zeros(2,2,2);
% Sigma(:,:,1) = [1 0; 0 1];
% Sigma(:,:,2) = [1 0; 0 1];
% disp(gaussian_pos_prob([1 0; 0 1], [0 1; 0 1], Sigma, [1/2 1/2]))

mu0 = [0; 0];
Sigma0 = [1 0; 0 1];
mu1 = [5; 5];
Sigma1 = [1 0; 0 1];
phi = 1/2;
plot_ex1(mu0, Sigma0, mu1, Sigma1, phi, 'Line', 1);

mu0 = [0; 0];
Sigma0 = [1 0; 0 1];
mu1 = [1; 1];
Sigma1 = [1 0; 0 1];
phi = 0.85;
plot_ex1(mu0, Sigma0, mu1, Sigma1, phi, 'Line (one side)', 2);

mu0 = [0; 0];
Sigma0 = [2 0; 0 5];
mu1 = [2; 2];
Sigma1 = [3 0; 0 5];
phi = 1/2;
plot_ex1(mu0, Sigma0, mu1, Sigma1, phi, 'Parabalic', 3);

mu0 = [0; 0];
Sigma0 = [2 0; 0 5];
mu1 = [0; 1];
Sigma1 = [3 0; 0 2];
phi = 1/2;
plot_ex1(mu0, Sigma0, mu1, Sigma1, phi, 'Hyperbola', 4);

mu0 = [0; 0];
Sigma0 = [2 0; 0 5];
mu1 = [0; 0];
Sigma1 = [3 0; 0 5];
phi = 1/2;
plot_ex1(mu0, Sigma0, mu1, Sigma1, phi, 'Two parallel lines.', 5);


mu0 = [0; 0];
Sigma0 = [1 0; 0 1];
mu1 = [0; 0];
Sigma1 = [5 0; 0 5];
phi = 1/2;
plot_ex1(mu0, Sigma0, mu1, Sigma1, phi, 'Circle', 6);

mu0 = [0; 0];
Sigma0 = [1 0; 0 1];
mu1 = [0; 0];
Sigma1 = [5 0; 0 2];
phi = 1/2;
plot_ex1(mu0, Sigma0, mu1, Sigma1, phi, 'Ellipsoid', 7);

mu0 = [0; 0];
Sigma0 = [1 0; 0 1];
mu1 = [0; 0];
Sigma1 = [1 0; 0 1];
phi = 1/4;
plot_ex1(mu0, Sigma0, mu1, Sigma1, phi, 'No boundary', 8);
