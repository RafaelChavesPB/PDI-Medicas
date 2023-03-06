import sys
import cv2
import numpy as np
from matplotlib import pyplot as plt, use
use('TkAgg')

img = cv2.imread(sys.argv[1]+sys.argv[2], cv2.IMREAD_GRAYSCALE) # Load a grayscale image

hist, bins = np.histogram(img.ravel(), 256, [0, 256]) # Compute the histogram

plt.bar(np.arange(len(hist)), hist)
plt.grid()
plt.title(sys.argv[2])
plt.xlim([0, 256])
plt.show()