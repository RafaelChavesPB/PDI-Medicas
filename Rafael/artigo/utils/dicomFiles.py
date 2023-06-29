import pydicom
from .imageProcessing import *

def getImgFromDicom(dicomFile):
    img = dicomFile.pixel_array
    return changeImgBitDepth(applyWindow(img, BEGIN_USEFUL_WINDOW, END_USEFUL_WINDOW, dtype=np.uint8), 8)
