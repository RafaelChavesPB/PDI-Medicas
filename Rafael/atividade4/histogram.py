import sys
import cv2
import numpy as np
from matplotlib import pyplot as plt, use
use('TkAgg')

if __name__ == '__main__':
    for filename in sys.argv[1:]:
        img = cv2.imread(filename)
        hsv_image = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        h_image = cv2.extractChannel(hsv_image, 0)
        hist, bins = np.histogram(h_image.ravel(), 12, [0, 180]) # Compute the histogram
        plt.bar(np.arange(len(hist)), hist)
        plt.grid()
        plt.title(filename)
        plt.xlim([0, 12])
        plt.show()