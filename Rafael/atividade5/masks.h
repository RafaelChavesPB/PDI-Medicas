#ifndef MASK
#define MASK
#include <opencv2/opencv.hpp>

namespace mask
{
    cv::Mat color_mask(cv::Mat &src, int begin, int end)
    {
        cv::Mat mask = cv::Mat::zeros(src.size(), CV_8U);
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols; j++)
            {
                int hue = src.at<cv::Vec3b>(i, j)[0];
                if (begin <= hue and hue < end)
                    mask.at<uchar>(i, j) = 255;
            }
        }
        return mask;
    }

    cv::Mat apply_color_mask(cv::Mat &src, cv::Mat &mask)
    {
        cv::Mat dst;
        src.copyTo(dst);
        for (int i = 0; i < src.rows; i++)
            for (int j = 0; j < src.cols; j++)
                src.at<cv::Vec3b>(i, j)[2] &= mask.at<uchar>(i, j);
        return dst;
    }
}

#endif