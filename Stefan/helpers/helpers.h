#include <string>
#include <sys/stat.h>
#include <opencv2/opencv.hpp>

#ifndef HELPERS_H
#define HELPERS_H

void clearString(std::string &s);
void createFolder(const char * filename);
void writeHistogramImage(cv::Mat &img, int histSize, float low, float high, int width, int height, std::string filename);
cv::Mat getModifiedGammaImage(cv::Mat &img, double gamma);
cv::Mat getBinarizedImage(cv::Mat &img, int threshold, bool inv);
unsigned char getGrayValueFromBGRPixel(cv::Vec3b pixel, bool mean);
double calculateSpatialInfo(cv::Mat imgGrayscale);

static std::string colorNames[12] = {
        "red",
        "orange",
        "yellow",
        "green_yellow",
        "green",
        "turquoise",
        "cyan",
        "blue",
        "violet",
        "magenta",
        "pink",
        "red_violet"
};

#endif