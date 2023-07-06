#include <opencv2/opencv.hpp>
#include <iostream>
#define PNEUIN "../resources/pneu.jpeg"
#define PNEUOUT "../generated/pneu.jpeg"
#define SOBEL "../generated/sobel.jpeg"
using namespace cv;

int best_threshold(Mat &src)
{
    int histogram[255] = {0};
    for (int i = 0; i < src.rows; i++)
        for (int j = 0; j < src.cols; j++)
            histogram[src.at<uchar>(i, j)]++;
    int half = (src.rows * src.cols) / 2;
    for (int i = 0, acc = 0; i < 255; i++)
    {
        acc += histogram[i];
        if (acc >= half)
        {
            return i;
        }
    }
    return 128;
}

void multiply(cv::Mat &src, cv::Mat &dst, cv::Mat &bin)
{
    
    dst = src.clone();
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            dst.at<uchar>(i, j) = (bin.at<uchar>(i, j) == 255) * src.at<uchar>(i, j);
        }
    }
}

void multiply_color(cv::Mat &src, cv::Mat &dst, cv::Mat &bin)
{
    
    dst = src.clone();
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            Vec3b pixel = src.at<Vec3b>(i, j);
            for(int k = 0; k < 3; k++){
                dst.at<Vec3b>(i, j)[k] = (bin.at<uchar>(i, j) == 255) * pixel[k];
            }
        }
    }
}

int main(int argc, char **argv)
{

    Mat rec = getStructuringElement(0, Size(3, 3));
    Mat rec_streched = getStructuringElement(0, Size(7, 3));
    Mat cross = getStructuringElement(1, Size(3, 3));
    Mat ellipse = getStructuringElement(1, Size(3, 3));
    Mat src, bin, edges, dst_erode_after, dst_dilate_after, dst_erode, dst_dilate, dst_dilatebin, src_col;
    int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_8U;
    src_col = imread(PNEUIN, IMREAD_COLOR);
    src = imread(PNEUIN, IMREAD_GRAYSCALE);
    GaussianBlur(src, src, Size(5, 5), 0, 0, BORDER_DEFAULT);
    Laplacian(src, bin, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
    imwrite("../generated/sobel.jpeg", bin);
    threshold(bin, bin, 80, 255, cv::THRESH_BINARY);
    imwrite("../generated/bin.jpeg", bin);
    // erode(bin, dst_erode, rec);
    // imwrite("../generated/erode_rec.jpeg", dst_erode);
    // erode(bin, dst_erode, cross);
    // imwrite("../generated/erode_cross.jpeg", dst_erode);
    // erode(bin, dst_erode, ellipse);
    // imwrite("../generated/erode_ellipse.jpeg", dst_erode);
    // dilate(bin, dst_dilate, rec);
    // imwrite("../generated/dilate_rec.jpeg", dst_dilate);
    // dilate(bin, dst_dilate, cross);
    // imwrite("../generated/dilate_cross.jpeg", dst_dilate);
    // dilate(bin, dst_dilate, ellipse);
    // imwrite("../generated/dilate_ellipse.jpeg", dst_dilate);
    // erode(dst_dilate, dst_erode_after, rec);
    // imwrite("../generated/after_erode_rec.jpeg", dst_erode_after);
    // erode(dst_dilate, dst_erode_after, cross);
    // imwrite("../generated/after_erode_cross.jpeg", dst_erode_after);
    // erode(dst_dilate, dst_erode_after, ellipse);
    // imwrite("../generated/after_erode_ellipse.jpeg", dst_erode_after);
    // dilate(dst_erode, dst_dilate_after, rec);
    // imwrite("../generated/after_dilate_rec.jpeg", dst_dilate_after);
    // dilate(dst_erode, dst_dilate_after, cross);
    // imwrite("../generated/after_dilate_cross.jpeg", dst_dilate_after);
    // dilate(dst_erode, dst_dilate_after, ellipse);
    // imwrite("../generated/after_dilate_ellipse.jpeg", dst_dilate_after);
    // Mat rec_edge, ellipse_edge, cross_edge;
    // erode(bin, edges, rec);
    // imwrite("../generated/edge_rec.jpeg", edges);
    // erode(bin, edges, cross);
    // imwrite("../generated/edge_cross.jpeg", edges);
    // erode(bin, edges, ellipse);
    // imwrite("../generated/edge_ellipse.jpeg", edges);
    // src = imread("../resources/fill.jpeg", IMREAD_GRAYSCALE);
    int i = 0;
    for (i = 0; i < 6; i++)
    {
        dilate(bin, bin, cross);
        imwrite(std::string("../generated/d") + std::to_string(i) + std::string(".jpeg"), bin);
    }
    for (i = 0; i < 12; i++)
    {
        erode(bin, bin, rec);
        imwrite(std::string("../generated/e") + std::to_string(i) + std::string(".jpeg"), bin);
    }
    for (i = 0; i < 8; i++)
    {
        dilate(bin, bin, rec_streched);
        imwrite(std::string("../generated/2d") + std::to_string(i) + std::string(".jpeg"), bin);
    }
    Mat dst;
    multiply_color(src_col, dst, bin);
    imwrite(std::string("../generated/mask.jpeg"), dst);
    std::cout << src.rows << ' ' << src.cols << ' ' << bin.rows << ' ' << bin.cols << std::endl; 
    return 0;
}