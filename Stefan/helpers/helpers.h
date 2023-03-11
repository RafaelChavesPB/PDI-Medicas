#include <string>
#include <sys/stat.h>
#include <opencv2/opencv.hpp>

#ifndef HELPERS_H
#define HELPERS_H

void clearString(std::string &s);
void createFolder(const char * filename);
cv::Mat getModifiedGammaImage(cv::Mat &img, double gamma);
void writeHistogramImage(cv::Mat &img, int histSize, float low, float high, int width, int height, std::string filename);
cv::Mat getBinarizedImage(cv::Mat &img, int threshold, bool inv);

#endif