function p = gaussian_pos_prob(X, Mu, Sigma, Phi)
%GAUSSIAN_POS_PROB Posterior probability of GDA.
%   p = GAUSSIAN_POS_PROB(X, Mu, Sigma) compute the posterior probability
%   of given N data points X using Gaussian Discriminant Analysis where the
%   K gaussian distributions are specified by Mu, Sigma and Phi.
%
%   Inputs:
%       'X'     - M-by-N matrix, N data points of dimension M.
%       'Mu'    - M-by-K matrix, mean of K Gaussian distributions.
%       'Sigma' - M-by-M-by-K matrix (yes, a 3D matrix), variance matrix of
%                   K Gaussian distributions.
%       'Phi'   - 1-by-K matrix, prior of K Gaussian distributions.
%
%   Outputs:
%       'p'     - N-by-K matrix, posterior probability of N data points
%                   with in K Gaussian distributions.

N = size(X, 2);
M = size(X, 1);
K = length(Phi);
p = zeros(N, K);
for n = 1:N
    for k = 1:K
        x = X(:,n);
        mu = Mu(:,k);
        sigma = Sigma(:,:,k);
        p(n, k) = Phi(k) * exp((x-mu)' * sigma^-1 * (x-mu)/-2) ./ ((2 * pi)^(M/2) * sqrt(det(sigma)));
    end
end
p = p ./ sum(p,2);
% Your code HERE
end

