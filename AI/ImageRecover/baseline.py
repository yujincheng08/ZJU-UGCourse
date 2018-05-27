import cv2 as cv
import sys
from scipy.stats import norm
import numpy as np

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Not enough arguments')
        exit(-1)
    img_path = sys.argv[1]
    corr_img = cv.imread(img_path)
    corr_img = corr_img.transpose() / 255  # h * w * c => c * w * h
    recover_img = corr_img.copy()
    noise_mask = corr_img == 0
    basis_num = 50
    channel = corr_img.shape[0]
    width = corr_img.shape[1]
    height = corr_img.shape[2]
    mu = np.linspace(0, 1, basis_num - 1)
    sigma = 0.01 * np.ones(basis_num - 1)
    x = np.arange(0, width)
    x = (x - np.min(x)) / (np.max(x))
    for c in range(channel):
        for h in range(height):
            line = corr_img[c, :, h]
            mask = noise_mask[c, :, h].transpose()
            train_index = np.nonzero(~mask)
            train_x = x[train_index]
            train_num = len(train_x)
            predict_index = np.nonzero(mask)
            predict_x = x[predict_index]
            predict_num = len(predict_x)

            train_phi = np.column_stack((np.ones([train_num, 1]), np.zeros([train_num, basis_num - 1])))
            train_phi[:, 1: basis_num] = norm.pdf((np.array([train_x] * (basis_num-1)).T - mu) / sigma) * np.sqrt(2 * np.pi)

            train_phi = np.matrix(train_phi)

            w = (train_phi.T * train_phi).I * train_phi.T * corr_img[c, train_index, h].T

            predict_phi = np.column_stack((np.ones([predict_num, 1]), np.zeros([predict_num, basis_num - 1])))
            predict_phi[:, 1: basis_num] = norm.pdf((np.array([predict_x] * (basis_num-1)).T - mu) / sigma) * np.sqrt(2 * np.pi)
            recover_img[c, predict_index, h] = w.transpose() * predict_phi.transpose()

    recover_img[np.nonzero(recover_img > 1)] = 1

    cv.namedWindow("Before", flags=0)
    cv.imshow("Before", corr_img.transpose())
    cv.namedWindow("After", flags=0)
    cv.imshow("After", recover_img.transpose())
    cv.waitKey(0)
    cv.destroyAllWindows()

