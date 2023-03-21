from sys import path
path.append('..')
from class_image import MyImage

img = MyImage('../images/beach.jpg')
img.getSingleColorImageFile([0,1,2])
img.size()
print("Informação espacial =", img.spatialInfo())