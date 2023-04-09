#include <opencv2/opencv.hpp>
#include <iostream>
#include "main.h"
#include "filters.h"
#include "algorithms.h"

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
    int window_size = std::stoi(argv[argc-1]);
    for (int i = 1; i < argc-1; i++)
    {
        filename = std::string(argv[i]);
        std::cout << filename << std::endl;
        image_name = treat::getImageName(filename);
        system((std::string("rm -r ") + prefix + image_name).c_str());
        system((std::string("mkdir ") + prefix + image_name).c_str());

        cv::Mat src = cv::imread(filename);
        cv::Mat hsv;
        cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
        algo::kmeans(hsv, 3);

        // // Original image
        // cv::Mat src = cv::imread(filename, cv::IMREAD_GRAYSCALE);
        // cv::imwrite(prefix + image_name + std::string("/gray.jpg"), src);
        // std::cout << "Original ... Done" << std::endl;

        // // Blured Image
        // cv::Mat blurred;
        // filter::applyBlurGrid(src, blurred, window_size);
        // cv::imwrite(prefix + image_name + std::string("/blurred.jpg"), blurred);
        // std::cout << "Blurred ... Done" << std::endl;

        // routine(src, filter::sobel_x, filter::sobel_y, std::string("sobel"), std::string("Sobel without blur"));
        // routine(blurred, filter::sobel_x, filter::sobel_y, std::string("sobel_blur"), std::string("Sobel with blur"));

        // routine(src, filter::roberts, std::string("roberts"), std::string("roberts without blur"));
        // routine(blurred, filter::roberts, std::string("roberts_blur"), std::string("roberts with blur"));

        // routine(src, filter::laplacian, std::string("laplacian"), std::string("laplacian without blur"));
        // routine(blurred, filter::laplacian, std::string("laplacian_blur"), std::string("laplacian with blur"));

        // routine(src, filter::prewit_x, filter::prewit_y, std::string("prewit"), std::string("prewit without blur"));
        // routine(blurred, filter::prewit_x, filter::prewit_y, std::string("prewit_blur"), std::string("prewit with blur"));

        // routine(src, filter::laplacian, std::string("laplacian_"), std::string("laplacian_ without blur"));
        // routine(blurred, filter::laplacian, std::string("laplacian_blur_"), std::string("laplacian_ with blur"));
    }

    return 0;
}