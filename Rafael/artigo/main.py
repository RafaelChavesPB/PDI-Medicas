import pydicom
import numpy as np
import matplotlib.pyplot as plt
import os


def showDicomImage(pixel_data):
    plt.imshow(pixel_data, cmap=plt.cm.bone)
    plt.show()

def detailDataset(filename):
    dataset = pydicom.dcmread(filename)
    dataset.decompress('pylibjpeg')
    print(dataset)

def iterateOverDatasets(path):
    os.path.isfile('dcm')
    for f in os.listdir(dcm_path):
        filename =  os.path.join(dcm_path, f)
        if filename[-3:] == 'dcm':
            detailDataset(filename)

if __name__ == '__main__':
    # dcm_path = './dicom-files/arq3/pt2'
    # iterateOverDatasets(dcm_path)
    dcm_path = './dicom-files/arq3/pt2/img3.dcm'
    detailDataset(dcm_path)