from sys import path
path.append('..')
from class_image import MyImage, MyPixel
import numpy as np
import cv2 as cv


def ssim(img1, img2):
    C1 = (0.01 * 255)**2
    C2 = (0.03 * 255)**2

    img1 = img1.astype(np.float64)
    img2 = img2.astype(np.float64)
    kernel = cv.getGaussianKernel(11, 1.5)
    window = np.outer(kernel, kernel.transpose())

    mu1 = cv.filter2D(img1, -1, window)[5:-5, 5:-5]  # valid
    mu2 = cv.filter2D(img2, -1, window)[5:-5, 5:-5]
    mu1_sq = mu1**2
    mu2_sq = mu2**2
    mu1_mu2 = mu1 * mu2
    sigma1_sq = cv.filter2D(img1**2, -1, window)[5:-5, 5:-5] - mu1_sq
    sigma2_sq = cv.filter2D(img2**2, -1, window)[5:-5, 5:-5] - mu2_sq
    sigma12 = cv.filter2D(img1 * img2, -1, window)[5:-5, 5:-5] - mu1_mu2

    ssim_map = ((2 * mu1_mu2 + C1) * (2 * sigma12 + C2)) / ((mu1_sq + mu2_sq + C1) *
                                                            (sigma1_sq + sigma2_sq + C2))
    return ssim_map.mean()


def calculate_ssim(img1, img2):
    '''
    Calculate SSIM
    the same outputs as MATLAB's
    img1, img2: [0, 255]
    '''
    if not img1.shape == img2.shape:
        raise ValueError('Input images must have the same dimensions.')
    if img1.ndim == 2:
        return ssim(img1, img2)
    elif img1.ndim == 3:
        if img1.shape[2] == 3:
            ssims = []
            for i in range(3):
                ssims.append(ssim(img1, img2))
            return np.array(ssims).mean()
        elif img1.shape[2] == 1:
            return ssim(np.squeeze(img1), np.squeeze(img2))
    else:
        raise ValueError('Wrong input image dimensions.')


a = MyImage('baixa_half.jpg')
b = MyImage('media_half.jpg')
c = MyImage('alta_half.jpg')

images = [a, b, c]
results = []
func_names = ['Mean', 'Median', 'Mode']
functions = [np.mean, np.median, MyPixel.mode]
interp_method = [cv.INTER_NEAREST, cv.INTER_LINEAR, cv.INTER_CUBIC]
interp_name = ['Nearest', 'Bilinear', 'Bicubic']

# Expansions
for img in images:
    name = img.name.split('_')[0]
    print("Imagem com", name, "spatial info ->", round(img.spatialInfo(), 2))
    big = cv.imread(name + '_big.jpg', cv.IMREAD_GRAYSCALE) #imagem para ser comparado os valores de psnr e ssim
    small = cv.imread(name + '_small.jpg', cv.IMREAD_GRAYSCALE) #imagem para ser comparado os valores de psnr e ssim
    for x in range(2):
        compimage = big if x < 1 else small
        op_name = "Expanded" if x < 1 else "Reduced"
        order = "Expand" if x < 1 else "Reduce"
        for funcName, func in zip(func_names, functions):
            print(order + " using " + funcName + '...')
            img.resize(2 if x < 1 else .5, 2 if x < 1 else .5, func, after=True if x < 1 else False, before=False if x < 1 else True) #resizes image
            img.writeLastResizedMatrix(name + funcName + op_name + '.png') #write to a file
            grayResized = cv.cvtColor(img.lastResizedMatrix, cv.COLOR_BGR2GRAY) # get gray image for comp methods
            psnr = round(cv.PSNR(compimage, grayResized), 2) #get PSNR 
            res_ssim = round(calculate_ssim(compimage, grayResized), 2) #get SSIM
            results.append((psnr, res_ssim, name + funcName + op_name))
        
        for interpName, interpMethod in zip(interp_name, interp_method):
            print(order + " using " + interpName + '...')
            h, w = compimage.shape[:2]
            resized_image = cv.resize(img.matrix, (w, h), interpolation=interpMethod) #resizes image
            cv.imwrite(name + interpName + op_name + '.png', resized_image) #write to a file
            grayResized = cv.cvtColor(resized_image, cv.COLOR_BGR2GRAY)
            psnr = round(cv.PSNR(compimage, grayResized), 2) #get PSNR 
            res_ssim = round(calculate_ssim(compimage, grayResized), 2) #get SSIM
            results.append((psnr, res_ssim, name + interpName + op_name))

print("Ordering results...")
sorted_results = sorted(results, key=lambda x: (x[0], x[1]), reverse=True)
for idx, val in enumerate(sorted_results):
    print(idx + 1, '. ', val[2], ' - ', val[0], ' - ', val[1], sep='')

