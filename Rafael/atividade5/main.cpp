#include <opencv2/opencv.hpp>
#include <iostream>
#include "main.h"
#include "filters.h"

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        std::string filename(argv[i]);
        std::string image_name = treat::getImageName(filename);
        std::string prefix("../generated/");
        system((std::string("rm -r ") + prefix + image_name).c_str());
        system((std::string("mkdir ") + prefix + image_name).c_str());
        
        
        // Original image
        cv::Mat src = cv::imread(filename, cv::IMREAD_GRAYSCALE);
        cv::imwrite(prefix + image_name + std::string("/gray.jpg"), src);
        std::cout << "Original ... Done" << std::endl;       
        
        // Blured Image
        cv::Mat blurred;
        pdi::applyBlurGrid(src, blurred, 8);
        cv::imwrite(prefix + image_name + std::string("/blurred.jpg"), blurred);
        std::cout << "Blurred ... Done" << std::endl;

        // Sobel without Blur
        cv::Mat sobel_x, sobel_y, sobel;
        edge::applyFilterGrid(src, sobel_x, edge::sobel_x, SOBEL_SIZE);
        cv::imwrite(prefix + image_name + std::string("/sobel_x.jpg"), sobel_x);
        edge::applyFilterGrid(src, sobel_y, edge::sobel_y, SOBEL_SIZE);
        cv::imwrite(prefix + image_name + std::string("/sobel_y.jpg"), sobel_y);
        sobel = sobel_x + sobel_y;
        cv::imwrite(prefix + image_name + std::string("/sobel.jpg"), sobel);
        std::cout << "Sobel without blur ... Done" << std::endl;

        // Sobel with Blur
        edge::applyFilterGrid(blurred, sobel_x, edge::sobel_x, SOBEL_SIZE);
        cv::imwrite(prefix + image_name + std::string("/sobel_x_blur.jpg"), sobel_x);
        edge::applyFilterGrid(blurred, sobel_y, edge::sobel_y, SOBEL_SIZE);
        cv::imwrite(prefix + image_name + std::string("/sobel_y_blur.jpg"), sobel_y);
        sobel = sobel_x + sobel_y;
        cv::imwrite(prefix + image_name + std::string("/sobel_blur.jpg"), sobel);
        std::cout << "Sobel with blur ... Done" << std::endl;
    }
    return 0;
}