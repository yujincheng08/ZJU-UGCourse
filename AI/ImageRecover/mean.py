import cv2 as cv
import sys


def mean(radius=1):
    total = 0
    count = 0
    for dx in range(-radius, 1 + radius):
        for dy in range(-radius, 1+radius):
            new_w = w + dx
            new_h = h + dy
            if 0 <= new_w < noise_mask.shape[1] \
                    and 0 <= new_h < noise_mask.shape[2] \
                    and not noise_mask[c, new_w, new_h]:
                total += corr_img[c, new_w, new_h]
                count += 1
    if count > 2 * radius:
        return total / count
    else:
        return mean(radius + 1)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Not enough arguments')
        exit(-1)
    img_path = sys.argv[1]
    corr_img = cv.imread(img_path)
    corr_img = corr_img.transpose()  # h * w * c => c * w * h
    recover_img = corr_img.copy()
    noise_mask = corr_img == 0
    for c in range(noise_mask.shape[0]):
        for w in range(noise_mask.shape[1]):
            for h in range(noise_mask.shape[2]):
                if noise_mask[c, w, h]:
                    recover_img[c, w, h] = mean()
    cv.namedWindow("Before", flags=0)
    cv.imshow("Before", corr_img.transpose())
    cv.namedWindow("After", flags=0)
    cv.imshow("After", recover_img.transpose())
    cv.waitKey(0)
    cv.destroyAllWindows()






