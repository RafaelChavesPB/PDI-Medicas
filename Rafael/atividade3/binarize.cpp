#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

void binarize(Mat &src, Mat &dst, int threshold)
{
    src.copyTo(dst);
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            dst.at<uchar>(i, j) = dst.at<uchar>(i, j) >= threshold ? 255 : 0;
        }
    }
}

void binarize_inv(Mat &src, Mat &dst, int threshold)
{
    src.copyTo(dst);
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            dst.at<uchar>(i, j) = dst.at<uchar>(i, j) >= threshold ? 0 : 255;
        }
    }
}


int main(int argc, char **argv)
{
    std::string path(argv[1]);
    std::string filename(argv[2]);
    int threshold = std::stoi(argv[3]);
    std::string prefix("../generated/");
    Mat image = imread(path + filename, IMREAD_GRAYSCALE);
    Mat binary_image, binary_inv_image;
    binarize(image, binary_image, threshold);
    binarize_inv(image, binary_inv_image, threshold);
    imwrite(prefix + filename, image);
    imwrite(prefix + std::string("binarized_inv.jpg"), binary_image);
    imwrite(prefix + std::string("binarized.jpg"), binary_image);
    return 0;
}