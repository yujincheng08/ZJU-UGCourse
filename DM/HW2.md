# Assignment #2
## 1. A Walk Through Line Models
- a. Perceptron
    1. If the target problem is linearly separable, then the train error is 0.
    Respectively when the size of training data is:
        - 10
            The average train error rate is 0. The average test error rate is 11.3190%. 
        - 100
            The average train error is 0. The average test error rate is 1.3350%.
    1. The average number of iterations should converge to half of the number of training data. Respectively when the size of training data is:
        - 10
            The average number of iterations is 5.664.
        - 100
            The average number of iterations is 45.427.
    1. If the training data is not linearly separable, the algorithm will loop endless.
- b. Linear Regression
    1. When the size of the linearly separable training set is 100, the train error rate is 3.891%, and the test error rate is 1.8%.
    1. When the size of the training set that is not linearly separable is 100, the train error rate is 13.6659%, and the test error rate is 5.2%.
    1. The training error rate is 49% and the test error rate is 54.96%.
    1. After transformation, the training error rate becomes 5.0% and the test error rate becomes 5.2%.
- c. Logistic Regression
    In our problem, the label is $\left\{1, -1\right\}$. So using sigmoid function to estimating the probabilities, we have:
    $$
    P(y_i=1\mid \mathbf{x}_i, \mathbf{w})=\frac{1}{1+e^{-\mathbf{w}^T\mathbf{x}_i}}\\
    P(y_i=-1\mid \mathbf{x}_i, \mathbf{w})=1-\frac{1}{1+e^{-\mathbf{w}^T\mathbf{x}_i}}=\frac{1}{1+e^{\mathbf{w}^T\mathbf{x}_i}}
    $$
    so:
    $$
    P(y_i\mid \mathbf{x}_i, \mathbf{w})=\frac{1}{1+e^{-y_i\mathbf{w}^T\mathbf{x}_i}}
    $$
    Using MLE, the log probability of dataset D is:
    $$
    l(D)=-\sum_i{\ln{\left(1+e^{-y_i\mathbf{w}^T\mathbf{x}_i}\right)}}
    $$
    To maximize $l(D)$ is to minimize $-l(D)$. So the error function is defined as:
    $$
    E(D)=\sum_i{\ln{\left(1+e^{-y_i\mathbf{w}^T\mathbf{x}_i}\right)}}
    $$
    Using gradient descent to minimize $E(D)$, suppose the learning rate is $\eta$, the gradient of $E(D)$ is:
    $$
    \nabla E(D)=\sum_i{\frac{-y_i\mathbf{x}_i}{1+e^{y_i\mathbf{w}^T\mathbf{x}_i}}}
    $$
    So applying gradient descent, the iteration step is:
    $$
    \mathbf{w}_{t+1} = \mathbf{w}_t - \eta\sum_i{\frac{-y_i\mathbf{x}_i}{1+e^{y_i\mathbf{w}^T\mathbf{x}_i}}}
    $$
    1. The training error rate of the linearly separable training set of size 100 is 0.76% and the test error rate is 1.0%.
    1. The training error rate of a noisy training set is 13.08%. And the test error rate is 6%.
- d. Support Vector Machine
    1. The training error rate is 0.0% and the test error rate is 3.3% with 30 linearly separable training data.
    1. For linearly separable training data with size 100, the training error rate is 0.005%, and the test error rate is 1.3%.
    1. The average number of support vectors are 5.0 when training data size is 100.

## 2. Regularization and Cross-Validation
- a. 
    1. The $\lambda$ chosen by LOOCV is 100.
    1. The $\sum_{i=1}^m\omega_i^2$ with regularization is 0.113724, and is 0.887821 without regularization.
    1. The training error rate is 0.0%, the test error rate is 6.4289%.
    Without regularization, the training error rate is 0.0%, the test error rate is 9.0909%.
- b.
    The $\lambda$ chosen by LOOCV is 0.001. The training error rate is 0.0%, the testing error is 6.0773%.
    Without regularization, the training error is 0.0%, the test error rate is 6.4289%.