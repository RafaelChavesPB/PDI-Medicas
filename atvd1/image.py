import cv2 as cv
from copy import deepcopy as dpcp

class MyImage:
    def __init__(self, filename):
        self.name, self.ext = filename.split(".")
        self.matrix = cv.imread(filename)
        self.red = self.onlyColor(2)
        self.green = self.onlyColor(1)
        self.blue = self.onlyColor(0)
        self.gray = self.toGray()

    # gera imagens cinza, azul, verde e vermelho
    def load(self):
        cv.imwrite('Gray-' + self.name + '.png', self.gray)
        cv.imwrite('Red-' + self.name + '.png', self.red)
        cv.imwrite('Blue-' + self.name + '.png', self.blue)
        cv.imwrite('Green-' + self.name + '.png', self.green)

    def toGray(self):
        m = dpcp(self.matrix) #copia da matriz original
        for i in range(len(m)):
            for j in range(len(m[i])):
                m[i, j] = self.getGrayValue(m[i, j]) #encontra o valor em tom de cinza correspondente 
        return m
    
    # retorna o valor correspondente em escala de cinza, ajustados os pesos
    def getGrayValue(self, rgb): 
        R = rgb[2]
        G = rgb[1]
        B = rgb[0]
        return [0.2989 * R + 0.5870 * G + 0.1140 * B]

    #retorna copia da matriz original com apenas a cor selecionada
    def onlyColor(self, color): 
        m = dpcp(self.matrix) #copia da matriz original
        for idx in [0, 1, 2]:
            if idx == color:
                continue
            m[:, :, idx] = 0
        return m
