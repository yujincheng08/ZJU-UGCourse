import cv2 as cv
import sys
import numpy as np
from sklearn.gaussian_process.gpr import GaussianProcessRegressor
from sklearn.gaussian_process.kernels import RBF


def fix_patch(c, w, h, patch_size, corr_img, recover_img):
    width = recover_img.shape[1]
    height = recover_img.shape[2]
    g = GaussianProcessRegressor(kernel=RBF(patch_size))
    if w + patch_size >= width:
        w = patch_size - width - 1
    if h + patch_size >= height:
        h = patch_size - height
    channel_img = corr_img[c, w:w+patch_size, h:h+patch_size]
    noise_mask = channel_img == 0
    sample_coordinates = np.nonzero(~noise_mask)
    if len(sample_coordinates[0]) == 0:
        return fix_patch(c, w, h, patch_size * 2, corr_img, recover_img)
    # if len(sample_coordinates[0]) < 2 * patch_size:
    #     return fix_patch(c, w, h, patch_size * 2, recover_img)
    train_x = np.array(sample_coordinates).transpose()
    train_y = channel_img[sample_coordinates]
    g.fit(train_x, train_y)
    predict_coordinates = np.nonzero(noise_mask)
    predict_x = np.array(predict_coordinates).transpose()
    predict_y = g.predict(predict_x)
    recover_img[c, w:w+patch_size, h:h+patch_size][predict_coordinates] = predict_y
    # train_y =


def main():
    if len(sys.argv) < 2:
        print('Not enough arguments')
        exit(-1)
    patch_size = 5
    img_path = sys.argv[1]
    corr_img = cv.imread(img_path)
    corr_img = corr_img.transpose()  # h * w * c => c * w * h
    recover_img = corr_img.copy()
    channel = corr_img.shape[0]
    width = corr_img.shape[1]
    height = corr_img.shape[2]
    for c in range(channel):
        for w in range(0, width, patch_size):
            for h in range(0, height, patch_size):
                fix_patch(c, w, h, patch_size, corr_img, recover_img)
                print("{}%".format(100 * (c * width * height + w * height + h * patch_size) / width / height / channel))
    cv.namedWindow("Before", flags=0)
    cv.imshow("Before", corr_img.transpose())
    cv.namedWindow("After", flags=0)
    cv.imshow("After", recover_img.transpose())
    cv.waitKey(0)
    cv.destroyAllWindows()


if __name__ == '__main__':
    main()

