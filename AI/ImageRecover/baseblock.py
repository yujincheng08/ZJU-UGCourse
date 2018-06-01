import cv2 as cv
import sys
import numpy as np
from scipy.stats import multivariate_normal


def main():
    if len(sys.argv) < 2:
        print('Not enough arguments')
        exit(-1)
    img_path = sys.argv[1]
    basis_num = 15
    corr_img = cv.imread(img_path)
    corr_img = corr_img.transpose() / 255  # h * w * c => c * w * h
    recover_img = corr_img.copy()
    noise_mask = corr_img == 0
    sigma = np.array([[0.01, 0.0], [0.0, 0.01]])
    (channel, width, height) = corr_img.shape
    x = np.arange(0, width)
    x = (x - np.min(x)) / (np.max(x))
    y = np.arange(0, height)
    y = (y - np.min(y)) / (np.max(y))
    mu_space = np.linspace(0, 1, basis_num)
    mu = np.array(np.meshgrid(mu_space, mu_space)).transpose()
    mu = mu.reshape(basis_num**2, 2)

    for c in range(channel):
        channel_img = corr_img[c]
        mask = noise_mask[c]
        (train_x_index, train_y_index) = np.nonzero(~mask)
        train_x = x[train_x_index]
        train_y = y[train_y_index]
        train_value = channel_img[train_x_index, train_y_index]
        train_num = len(train_value)
        train_coordinates = np.concatenate([[train_x], [train_y]]).transpose()
        train_coordinates = np.array([train_coordinates] * basis_num ** 2).swapaxes(0, 1)
        train_coordinates = train_coordinates - mu
        train_coordinates = train_coordinates.reshape(train_num * basis_num ** 2, 2)
        train_phi = np.column_stack((np.ones([train_num, 1]), np.zeros([train_num, basis_num**2])))
        train_phi[:, 1:basis_num**2 + 1] = multivariate_normal.pdf(train_coordinates, cov=sigma)\
            .reshape(train_num, basis_num**2)

        train_phi = np.matrix(train_phi)

        w = (train_phi.T * train_phi).I * train_phi.T * np.matrix(train_value).T

        (predict_x_index, predict_y_index) = np.nonzero(mask)

        predict_x = x[predict_x_index]
        predict_y = y[predict_y_index]

        predict_num = len(predict_x)

        predict_coordinate = np.concatenate([[predict_x], [predict_y]]).transpose()
        predict_coordinate = np.array([predict_coordinate] * basis_num ** 2).swapaxes(0, 1)
        predict_coordinate = predict_coordinate - mu
        predict_coordinate = predict_coordinate.reshape(predict_num * basis_num ** 2, 2)
        predict_phi = np.column_stack((np.ones([predict_num, 1]), np.zeros([predict_num, basis_num**2])))
        predict_phi[:, 1:basis_num**2 + 1] = multivariate_normal.pdf(predict_coordinate, cov=sigma) \
            .reshape(predict_num, basis_num**2)
        recover_img[c][predict_x_index, predict_y_index] = (w.transpose() * predict_phi.transpose())

    recover_img[np.nonzero(recover_img > 1)] = 1
    recover_img[np.nonzero(recover_img < 0)] = 0

    cv.namedWindow("Before", flags=0)
    cv.imshow("Before", corr_img.transpose())
    cv.namedWindow("After", flags=0)
    cv.imshow("After", recover_img.transpose())
    cv.waitKey(0)
    cv.destroyAllWindows()
    cv.imwrite(img_path+'_baseblock.png', recover_img.transpose() * 255)


if __name__ == '__main__':
    main()
