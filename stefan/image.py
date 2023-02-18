import cv2 as cv
import numpy as np
from copy import deepcopy as dpcp

class MyImage:
    """
    A custom class for working with images.

    Attributes:
    - name (str): the name of the file without extension
    - ext (str): the extension of the file
    - matrix (np.ndarray): the color image matrix
    - red (np.ndarray): the matrix of the red channel
    - green (np.ndarray): the matrix of the green channel
    - blue (np.ndarray): the matrix of the blue channel
    - gray (np.ndarray): the matrix of the grayscale image

    Methods:
    - __init__(filename: str): initializes the MyImage object with the file specified by filename.
    - getSingleColorImageFile(want: list): returns single color images for the given indices, and saves them as separate files.
    - getGrayValue(rgb: List[int]): returns a balanced grayscale value given a list of BGR components.
    - getOnlyColor(color: int): returns the color matrix with only one color channel.
    - getOnlyGrayImage(): returns the grayscale matrix with only one channel.
    - getOnlyBlueImage(): returns the blue channel matrix.
    - getOnlyGreenImage(): returns the green channel matrix.
    - getOnlyRedImage(): returns the red channel matrix.
    """


    def __init__(self, filename):
        # constructor to initialize instance variables
        self.name, self.ext = filename.split(".")  # split file name and extension
        self.matrix = cv.imread(filename) # read the image file into a matrix
        self.blue = None # variable to store the blue color image
        self.green = None # variable to store the green color image
        self.red = None # variable to store the red color image
        self.gray = None # variable to store the grayscale image
        self.gradientMagnitude = None # variable to store how abruptly the pixels itensity changes from one to another
        self.meanGradientMagnitude = None # variable to store the mean of gradient magnitude
        self.rmsGradientMagnitude = None # variable to store the rms value of gradient magnitude
        self.stdvGradientMagnitude = None # variable to store the standard deviation value of gradient magnitude

    def getSingleColorImageFile(self, want : list):
        """
        This method creates single color images of the original image.
        It takes a list of integers as input which specifies which color images are to be created.

        want: list of integers. This parameter specifies which color images to be created. Each element of the list
        should be one of the following: 0 for blue, 1 for green, 2 for red, or any other value for gray.

        Example usage: getSingleColorImages([0, 2]) # creates only blue and red images

        """
        flag = False
        for number in want:
            # check the input list for the requested color images
            if not flag and (number < 0 or number > 2):
                self.gray = self.getOnlyGrayImage()
                cv.imwrite('Gray-' + self.name + '.' + self.ext, self.gray) # write the grayscale image to a file
                flag = True
            elif number == 0:
                self.blue = self.getOnlyBlueImage()
                cv.imwrite('Blue-' + self.name + '.' + self.ext, self.blue) # write the blue color image to a file
            elif number == 1:
                self.green = self.getOnlyGreenImage()
                cv.imwrite('Green-' + self.name + '.' + self.ext, self.green) # write the green color image to a file
            elif number == 2:
                self.red = self.getOnlyRedImage()
                cv.imwrite('Red-' + self.name + '.' + self.ext, self.red) # write the red color image to a file
            

    def getGrayValue(self, bgr):
        """
        This method takes an BGR color value and returns the corresponding grayscale value.

        bgr: list of 3 integers. Each element should be an integer between 0 and 255, representing the blue,
        green, and red color values of a pixel in the image.

        returns: a rounded single float representing the corresponding grayscale value.
        """
        R = bgr[2]
        G = bgr[1]
        B = bgr[0]
        return np.float32(0.2989 * R + 0.5870 * G + 0.1140 * B)

    def getOnlyColor(self, color):
        """
        Returns a matrix with only one color, specified by the `color` parameter (0 for blue, 1 for green, 2 for red).
        If an invalid color is specified, returns a grayscale image instead.
        """
        # If an invalid color is specified, return a grayscale image
        if color < 0 or color > 2:
            return self.getOnlyGrayImage()

        # Make a copy of the original matrix to avoid modifying the original image
        m = dpcp(self.matrix)
 
        # Loop over all the pixels in the matrix and set all color components except the specified one to zero
        for idx in [0, 1, 2]:
            if idx == color:
                continue
            m[:, :, idx] = 0
        return m

    def getOnlyGrayImage(self):
        """
         Returns a color matrix with the value of BGR components turned into a single, balanced grayscale value. 
        """
        # Create a matrix full of zeros with shape identical to the original matrix to avoid modifying the original image
        m = np.zeros((len(self.matrix), len(self.matrix[0])), dtype=np.float32)
        
        # Loop over all the pixels in the matrix and calculate their grayscale value
        for i in range(len(m)):
            for j in range(len(m[i])):
                # Call the getGrayValue method to calculate the balanced grayscale value
                m[i, j] = self.getGrayValue(self.matrix[i, j])

        return m
    
    def getOnlyBlueImage(self):
        """Returns color matrix with green and red components of all pixels zeroed"""
        return self.getOnlyColor(0)
    
    def getOnlyGreenImage(self):
        """Returns color matrix with blue and red components of all pixels zeroed"""
        return self.getOnlyColor(1)
    
    def getOnlyRedImage(self):
        """Returns color matrix with blue and green components of all pixels zeroed"""
        return self.getOnlyColor(2)

    def size(self):
        print(len(self.matrix), "X", len(self.matrix[0]))

    def spatialInfo(self):
        """
        This method calculates the standard deviation of the gradient magnitude of an image.
        It first checks if the grayscale image is available, and if not, it obtains it.
        Then it calculates the horizontal and vertical gradients of the image using Sobel operators.
        The magnitude of the gradient is calculated as the square root of the sum of the squares of the horizontal and vertical gradients. 
        The mean and root mean squared values of the gradient magnitude are calculated, followed by the standard deviation of the gradient magnitude.
        Finally, returns the standard deviation of the gradient magnitude.
        """
        # Check if the grayscale image is already available, otherwise obtain it
        if self.gray is None:
            self.gray = self.getOnlyGrayImage()

        # Calculate the horizontal (x) gradient of the image
        derivative_x = cv.Sobel(self.gray, cv.CV_64F, 1, 0, ksize=1)

        # Calculate the vertical (y) gradient of the image
        derivative_y = cv.Sobel(self.gray, cv.CV_64F, 0, 1, ksize=1)

        # Calculate the magnitude of the gradient of the image
        self.gradientMagnitude = gradient_magnitude = np.sqrt(np.square(derivative_x) + np.square(derivative_y))

        # Calculate the mean of the gradient magnitude
        self.meanGradientMagnitude = mean_gradient_magnitude = np.sum(gradient_magnitude) / (gradient_magnitude.shape[0] * gradient_magnitude.shape[1])

        # Calculate the root mean squared of the gradient magnitude
        self.rmsGradientMagnitude = root_mean_squared_gradient_magnitude = np.sqrt(np.sum(gradient_magnitude ** 2) / (gradient_magnitude.shape[0] * gradient_magnitude.shape[1]))

        # Calculate the standard deviation of the gradient magnitude
        self.stdvGradientMagnitude = gradient_magnitude_std_deviation = np.sqrt(np.sum(gradient_magnitude ** 2 - mean_gradient_magnitude ** 2) / (gradient_magnitude.shape[0] * gradient_magnitude.shape[1]))

        return gradient_magnitude_std_deviation
