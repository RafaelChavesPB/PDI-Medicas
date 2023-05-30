import pydicom
import numpy as np
import matplotlib.pyplot as plt
import sys
import os

# load the DICOM files
dcm_path = './dicom-files/arq3/pt2'

files = []

for f in os.listdir(dcm_path):
    filename = os.path.join(dcm_path, f)
    if filename[-3:] == 'dcm':
        files.append(pydicom.dcmread(filename))

# skip files with no SliceLocation (eg scout views)
slices = []
skipcount = 0
for f in files:
    if hasattr(f, 'SliceLocation'):
        slices.append(f)
    else:
        skipcount = skipcount + 1

# ensure they are in the correct order
slices = sorted(slices, key=lambda s: s.SliceLocation)

# pixel aspects, assuming all slices are the same
ps = slices[0].PixelSpacing
ss = slices[0].SliceThickness
ax_aspect = ps[1]/ps[0]
sag_aspect = ps[1]/ss
cor_aspect = ss/ps[0]

# create 3D array
img_shape = list(slices[0].pixel_array.shape)
img_shape.append(len(slices))
img3d = np.zeros(img_shape)

# fill 3D array with the images from the files
for i, s in enumerate(slices):
    img2d = s.pixel_array
    img3d[:, :, i] = img2d

factor = 512/img3d.shape[2]
final = np.zeros((512, 512, 512))

for i in range(512):
    p = int(i / factor)
    final[:, :, i] = img3d[:, :, min(p, len(slices)-1)]

# plot 3 orthogonal slices

idx = 150

plt.axes().set_aspect(ax_aspect)
plt.imshow(final[:, :, idx], cmap=plt.cm.bone)
plt.show()

plt.axes().set_aspect(sag_aspect)
plt.imshow(final[:, idx, :], cmap=plt.cm.bone)
plt.show()

plt.axes().set_aspect(cor_aspect)
plt.imshow(final[idx, :, :].T, cmap=plt.cm.bone)
plt.show()

