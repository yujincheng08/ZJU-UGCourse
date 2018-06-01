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