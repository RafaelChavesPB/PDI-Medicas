#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int changePixelGamma(uchar pixel, double gamma)
{
    const double max_intensity = 255;
    const double pixel_normalized = static_cast<int>(pixel) / max_intensity;
    return max_intensity * pow(pixel_normalized, gamma);
}

void changeImageGamma(Mat &src, Mat &dst, double gamma)
{
    src.copyTo(dst);
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            dst.at<uchar>(i, j) = changePixelGamma(src.at<uchar>(i, j), gamma);
        }
    }
}

int main(int argc, char **argv)
{
    std::string path(argv[1]);
    std::string filename(argv[2]);
    std::string prefix("../generated/");
    Mat image = imread(path + filename, IMREAD_GRAYSCALE);
    Mat gamma0_5, gamma2_0;
    changeImageGamma(image, gamma0_5, 0.5);
    changeImageGamma(image, gamma2_0, 2);
    imwrite(prefix + filename, image);
    imwrite(prefix + std::string("gamma0_5.jpg"), gamma0_5);
    imwrite(prefix + std::string("gamma2_0.jpg"), gamma2_0);
    return 0;
}