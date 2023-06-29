import numpy as np
import matplotlib.pyplot as plt
import cv2

def plotImage(img, size=(5, 5), title='window'):
    plt.figure(figsize=size)
    plt.imshow(img, cmap='gray')

def showImage(img, title=''):
    cv2.imshow(title, img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def compareTwoImages(a, b, size=(10, 10)):
    fig, ax = plt.subplots(nrows=1, ncols=2, figsize=size)
    ax[0].imshow(a, cmap='gray')
    ax[1].imshow(b, cmap='gray')
    plt.show()


def plotHistogram(src):
    val, count = np.unique(src, return_counts=True)
    plt.plot(val, count)
    plt.show()
