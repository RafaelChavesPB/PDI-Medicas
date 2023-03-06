#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

void window_filter(Mat &src, Mat &dst, int lower, int upper)
{
    src.copyTo(dst);
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            if(dst.at<uchar>(i, j) > upper or dst.at<uchar>(i, j) < lower)
                dst.at<uchar>(i, j) = 255;
        }
    }
}

int main(int argc, char **argv)
{
    std::string path(argv[1]);
    std::string filename(argv[2]);
    int lower = std::stoi(argv[3]);
    int upper = std::stoi(argv[4]);
    std::string prefix("../generated/");
    Mat image = imread(path + filename, IMREAD_GRAYSCALE);
    Mat binary_image;
    window_filter(image, binary_image, lower, upper);
    imwrite(prefix + filename, image);
    imwrite(prefix + std::string("filtered.jpg"), binary_image);
    return 0;
}