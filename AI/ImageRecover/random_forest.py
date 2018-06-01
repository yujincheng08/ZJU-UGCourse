from sklearn.ensemble import RandomForestRegressor
import cv2 as cv

import sys
import numpy as np


def fix_patch(c, w, h, patch_size, corr_img, recover_img):
    width = recover_img.shape[1]
    height = recover_img.shape[2]
    rfr = RandomForestRegressor(n_estimators=10)
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
    rfr.fit(train_x, train_y)
    predict_coordinates = np.nonzero(noise_mask)
    predict_x = np.array(predict_coordinates).transpose()
    predict_y = rfr.predict(predict_x)
    recover_img[c, w:w+patch_size, h:h+patch_size][predict_coordinates] = predict_y
    # train_y =


def optimize(corr_img, recover_img):
    radius = 2
    (channel, width, height) = recover_img.shape
    for c in range(channel):
        for w in range(width):
            for h in range(height):
                if corr_img[c, w, h] != 0:
                    continue
                total = 0
                count = 0
                for dx in range(-radius, 1 + radius):
                    for dy in range(-radius, 1 + radius):
                        new_w = w + dx
                        new_h = h + dy
                        if 0 <= new_w < width \
                                and height > new_h >= 0 \
                                and 0 != new_h != new_w:
                            total += recover_img[c, new_w, new_h]
                            count += 1
                if abs(total / count - recover_img[c, w, h]) > 0.2:
                    recover_img[c, w, h] = total / count


def fix_global(corr_img, recover_img):
    rfr = RandomForestRegressor(n_estimators=50)
    noise_mask = corr_img == 0
    sample_coordinates = np.nonzero(~noise_mask)
    # if len(sample_coordinates[0]) < 2 * patch_size:
    #     return fix_patch(c, w, h, patch_size * 2, recover_img)
    train_x = np.array(sample_coordinates).transpose()
    train_y = corr_img[sample_coordinates]
    rfr.fit(train_x, train_y)
    predict_coordinates = np.nonzero(noise_mask)
    predict_x = np.array(predict_coordinates).transpose()
    predict_y = rfr.predict(predict_x)
    recover_img[predict_coordinates] = predict_y


def main():
    if len(sys.argv) < 2:
        print('Not enough arguments')
        exit(-1)
    img_path = sys.argv[1]
    corr_img = cv.imread(img_path)
    corr_img = corr_img.transpose() / 255  # h * w * c => c * w * h
    recover_img = corr_img.copy()
    '''
    for c in range(channel):
        for w in range(0, width, patch_size):
            for h in range(0, height, patch_size):
                fix_patch(c, w, h, patch_size, corr_img, recover_img)
                print("{}%".format(100 * (c * width * height + w * height + h * patch_size) / width / height / channel))
        '''
    fix_global(corr_img, recover_img)

    recover_img[np.nonzero(recover_img > 1)] = 1
    recover_img[np.nonzero(recover_img < 0)] = 0

    cv.namedWindow("Before", flags=0)
    cv.imshow("Before", corr_img.transpose())

    cv.namedWindow("After", flags=0)
    cv.imshow("After", recover_img.transpose())
    cv.waitKey(0)
    cv.destroyAllWindows()
    cv.imwrite(img_path + '_rfr.png', recover_img.transpose() * 255)


if __name__ == '__main__':
    main()
