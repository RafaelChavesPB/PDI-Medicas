#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

void getImageLayers(Mat &src, Mat *dst)
{
    for (int bit = 0; bit < 8; bit++)
    {
        src.copyTo(dst[bit]);
        int mask = 1 << bit;
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols; j++)
            {
                dst[bit].at<uchar>(i, j) = src.at<uchar>(i, j) & mask;
            }
        }
    }
}

void joinImageLayers(Mat *src, Mat &dst, int mymask)
{
    dst = Mat::zeros(src[0].size(), src[0].type());
    for (int bit = 0; bit < 8; bit++)
    {
        int mask = 1 << bit;
        if (mask & mymask)
        {
            for (int i = 0; i < dst.rows; i++)
            {
                for (int j = 0; j < dst.cols; j++)
                {
                    dst.at<uchar>(i, j) |= src[bit].at<uchar>(i, j);
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    std::string path(argv[1]);
    std::string filename(argv[2]);
    std::string prefix("../generated/");
    Mat image = imread(path + filename, IMREAD_GRAYSCALE);
    Mat layers[8];
    Mat joinedImage;
    getImageLayers(image, layers);
    joinImageLayers(layers, joinedImage, 127);
    for (int i = 0; i < 8; i++)
        imwrite(prefix + std::to_string(i) + std::string("layer.jpg"), layers[i]);
    imwrite(prefix + filename, image);
    imwrite(prefix + std::string("joined.jpg"), joinedImage);
    return 0;
}