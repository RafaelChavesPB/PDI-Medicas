import pydicom
import os
from .imageProcessing import *

def readDicomFiles(path):
    files = []

    for f in os.listdir(path):
        filename = os.path.join(path, f)
        if filename[-3:] == 'dcm':
            files.append(pydicom.dcmread(filename))

    slices = []
    skipcount = 0
    for f in files:
        if hasattr(f, 'SliceLocation'):
            slices.append(f)
        else:
            skipcount = skipcount + 1
    return list(sorted(slices, key=lambda s: s.SliceLocation))

def getImgFromDicomFile(dicomFile):
    img = dicomFile.pixel_array
    img[img < 0] = 0
    return changeImgBitDepth(applyWindow(img, BEGIN_USEFUL_WINDOW, END_USEFUL_WINDOW, dtype=np.uint8), 8)
