# Assignment #4
## 1. Spectral Clustering
- a 
    The result of my clustering is shown as follow:
    - kmeans:
        ![](assets/4-1-a-1.svg)
    - Spectral Clustering:
        ![](assets/4-1-a-2.svg)
        I use 200 neighbors and 0.5 threshold in the Spectral Clustering.
- b
    I run both kmeans and spectral clustering for 50 times.
    And as a result, the accuracy of the kmeans is 50.153%, and the the normalized mutual information metric is 0.38384.
    The accuracy of Spectral Clustering is 71.759% and the normalized mutual information metric is 0.27937.

 
## 2. Principal Component Analysis
- a
    The result after using PCA to recover the non-rotated CAPTCHA image:
    ![](assets/4-2-a-1.gif)
    ![](assets/4-2-a-2.gif)
    ![](assets/4-2-a-3.gif)
    ![](assets/4-2-a-4.gif)
    ![](assets/4-2-a-5.gif)
- b
    - i.
        The Eigenface are shown as follow:
        ![](assets/4-2-b-1-1.svg)
    - ii.
        I use 1-nn in this experiment. And the testing error rate of each number of reduced dimensionality are as follows:
        - 8
            75.5%
        - 16
            80.0%
        - 32
            82.0%
        - 64
            85.0%
        - 128
            85.0%

    - iii.
        Yes. It case a certain extent loss of information. But the loss is regardless because it doesn't have huge performance effect.
        The testing error rate of each number of reduced dimensionality are as follows:
        - 8
            ![](assets/4-2-b-3-1.svg)
        - 16
            ![](assets/4-2-b-3-2.svg)
        - 32
            ![](assets/4-2-b-3-3.svg)
        - 64
            ![](assets/4-2-b-3-4.svg)
        - 128
            ![](assets/4-2-b-3-5.svg)
- c
    I tried to use LDA for classification after dimension reduction. The dimension I use is the number of class minus 1. In this dataset, it's 39.
    The accuracy is 94% with 1-nn. It's much better than using PCA (85%) and raw data (also 85%).