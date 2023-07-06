from os import makedirs
from os import listdir
from shutil import copyfile
from random import seed
from random import random
from utils.dicomFiles import *
import cv2

dataset_home = 'dicom-files/'

dicom_home = 'dicom-files/'
image_home = 'files/'
image_subdirs = ['train/', 'test/', 'validation/']
labeldirs = ['ok/', 'avc/']

makedirs(image_home, exist_ok=True)
for subdir in image_subdirs:
    labeldirs = ['ok/', 'avc/']
    for labldir in labeldirs:
        newdir = image_home + subdir + labldir
        makedirs(newdir, exist_ok=True)

seed(1)
train_ratio = 0.70
val_ratio = 0.85
for labldir in labeldirs:
    images = list(map(getImgFromDicomFile,readDicomFiles(dicom_home+labldir)))
    for i in range(len(images)):
        curr = random()
        if curr < train_ratio:
            cv2.imwrite(f'{image_home}train/{labldir}img{i+1}.jpg', images[i])
        elif curr < val_ratio:
            cv2.imwrite(f'{image_home}validation/{labldir}img{i+1}.jpg', images[i])
        else:
            cv2.imwrite(f'{image_home}test/{labldir}img{i+1}.jpg', images[i])