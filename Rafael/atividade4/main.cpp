#include <opencv2/opencv.hpp>
#include <iostream>
#include "main.h"

int main(int argc, char **argv)
{

    for (int i = 1; i < argc; i++)
    {
        std::string filename(argv[i]);
        std::string image_name = treat::getImageName(filename);
        std::string prefix("../generated/");
        system((std::string("mkdir ") + prefix + image_name).c_str());
        cv::Mat src = cv::imread(filename);
        // Mean
        cv::Mat gray_image_mean;
        pdi::convToGray(src, gray_image_mean, pdi::BGRtoGrayMean);
        cv::imwrite(prefix + image_name + std::string("/gray_mean.jpg"), gray_image_mean);
        // Weighted Mean
        cv::Mat gray_image_weighted;
        pdi::convToGray(src, gray_image_weighted, pdi::BGRtoGrayWeighted);
        cv::imwrite(prefix + image_name + std::string("/gray_weighted.jpg"), gray_image_weighted);
        // HSV image
        cv::Mat hsv_image;
        cv::cvtColor(src, hsv_image, cv::COLOR_BGR2HSV_FULL);
        cv::imwrite(prefix + image_name + std::string("/hsv.jpg"), hsv_image);
        // Saving H, S and V separeted
        cv::Mat h_image(hsv_image.size(), CV_8UC1), s_image(hsv_image.size(), CV_8UC1), v_image(hsv_image.size(), CV_8UC1);
        cv::extractChannel(hsv_image, h_image, 0);
        cv::imwrite(prefix + image_name + std::string("/h.jpg"), h_image);
        cv::extractChannel(hsv_image, s_image, 1);
        cv::imwrite(prefix + image_name + std::string("/s.jpg"), s_image);
        cv::extractChannel(hsv_image, v_image, 2);
        cv::imwrite(prefix + image_name + std::string("/v.jpg"), v_image);


    }
    return 0;
}