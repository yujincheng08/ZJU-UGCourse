import cv2 as cv

import sys
import numpy as np

from sklearn.ensemble import GradientBoostingRegressor


def fix_global(corr_img, recover_img):
    g = GradientBoostingRegressor(loss='ls', max_depth=4, n_estimators=500)
    noise_mask = corr_img== 0
    sample_coordinates = np.nonzero(~noise_mask)
    # if len(sample_coordinates[0]) < 2 * patch_size:
    #     return fix_patch(c, w, h, patch_size * 2, recover_img)
    train_x = np.array(sample_coordinates).transpose()
    train_y = corr_img[sample_coordinates]
    g.fit(train_x, train_y)
    predict_coordinates = np.nonzero(noise_mask)
    predict_x = np.array(predict_coordinates).transpose()
    predict_y = g.predict(predict_x)
    recover_img[predict_coordinates] = predict_y


def main():
    if len(sys.argv) < 2:
        print('Not enough arguments')
        exit(-1)
    img_path = sys.argv[1]
    corr_img = cv.imread(img_path)
    corr_img = corr_img.transpose()  # h * w * c => c * w * h
    recover_img = corr_img.copy()
    '''
    for c in range(channel):
        for w in range(0, width, patch_size):
            for h in range(0, height, patch_size):
                fix_patch(c, w, h, patch_size, corr_img, recover_img)
                print("{}%".format(100 * (c * width * height + w * height + h * patch_size) / width / height / channel))
        '''
    fix_global(corr_img, recover_img)

    recover_img[np.nonzero(recover_img > 255)] = 255
    recover_img[np.nonzero(recover_img < 0)] = 0

    recover_img = cv.medianBlur(recover_img, 3)

    cv.namedWindow("Before", flags=0)
    cv.imshow("Before", corr_img.transpose())
    cv.namedWindow("After", flags=0)
    cv.imshow("After", recover_img.transpose())
    cv.waitKey(0)
    cv.destroyAllWindows()


if __name__ == '__main__':
    main()