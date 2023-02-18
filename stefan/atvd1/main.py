from sys import path
path.append('..')
from image import MyImage

img = MyImage('beach.jpg')
img.getSingleColorImageFile([0,1,2])
img.size()
print("Informação espacial =", img.spatialInfo())