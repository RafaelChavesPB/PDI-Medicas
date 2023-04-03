#include <opencv2/opencv.hpp>
#include <iostream>
#include "main.h"
#include "filters.h"

std::string filename, image_name, prefix("../generated/");

void routine(cv::Mat src, int mask_x[3][3], int mask_y[3][3], std::string maskname, std::string description)
{
    cv::Mat dst_x, dst_y, two_axis;
    filter::applyFilterGrid(src, dst_x, mask_x);
    cv::imwrite(prefix + image_name + std::string("/") + maskname + std::string("_x.jpg"), dst_x);

    filter::applyFilterGrid(src, dst_y, mask_y);
    cv::imwrite(prefix + image_name + std::string("/") + maskname + std::string("_y.jpg"), dst_y);

    two_axis = dst_x + dst_y;
    cv::imwrite(prefix + image_name + std::string("/") + maskname + std::string(".jpg"), two_axis);
    std::cout << description << " ... Done" << std::endl;
}

void routine(cv::Mat src, int mask[3][3], std::string maskname, std::string description)
{
    cv::Mat dst;
    filter::applyFilterGrid(src, dst, mask);
    cv::imwrite(prefix + image_name + std::string("/") + maskname + std::string(".jpg"), dst);
    std::cout << description + " ... Done" << std::endl;
}

int main(int argc, char **argv)
{
    for (int i = 1; i < argc - 1; i++)
    {
        int window_size = std::stoi(argv[argc - 1]);
        filename = std::string(argv[1]);
        image_name = treat::getImageName(filename);
        system((std::string("rm -r ") + prefix + image_name).c_str());
        system((std::string("mkdir ") + prefix + image_name).c_str());

        // Original image
        cv::Mat src = cv::imread(filename, cv::IMREAD_GRAYSCALE);
        cv::imwrite(prefix + image_name + std::string("/gray.jpg"), src);
        std::cout << "Original ... Done" << std::endl;

        cv::Mat blurred;
        filter::applyBlurGrid(src, blurred, window_size);
        cv::imwrite(prefix + image_name + std::string("/blurred.jpg"), blurred);
        std::cout << "Blurred ... Done" << std::endl;
    }

    return 0;
}