#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;

void getImageLayers(cv::Mat &src, cv::Mat *dst)
{
    for (int bit = 0; bit < 8; bit++)
    {
        src.copyTo(dst[bit]);
        int mask = 1 << bit;
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols; j++)
            {
                dst[bit].at<uchar>(i, j) = (src.at<uchar>(i, j) & mask) << 7 - bit;
            }
        }
    }
}

int main()
{
    char filename[100];

    Mat gray, bgr, hsv;
    gray = imread("../images/ball.jpg", IMREAD_GRAYSCALE);
    bgr = imread("../images/ball.jpg", IMREAD_COLOR);
    cvtColor(bgr, hsv, COLOR_BGR2HSV);
    imwrite("../images/gray.jpg", gray);
    imwrite("../images/hsv.jpg", hsv);
    return 0;
}

// // GET BIT LAYERS:
// Mat layers[8];
// getImageLayers(gray, layers);
// for(int i = 0; i < 8; i++){
//     sprintf(filename, "../images/layers/%d.jpg",i);
//     imwrite(filename, layers[i]);
// }

// // GET BGR CHANNELS
// Mat channels[3];
// char labels[] = "BGR";
// split(bgr, channels);
// for(int i = 0; i < 3; i++){
//     sprintf(filename, "../images/channels/bgr/%c.jpg",labels[i]);
//     imwrite(filename, channels[i]);
// }

// // GET HSV CHANNELS
// Mat channels[3];
// char labels[] = "HSV";
// split(hsv, channels);
// for(int i = 0; i < 3; i++){
//     sprintf(filename, "../images/channels/hsv/%c.jpg",labels[i]);
//     imwrite(filename, channels[i]);
// }

// // GET BGR CHANNELS LAYERS
// Mat layers[8];
// Mat channels[3];
// char labels[] = "BGR";
// split(bgr, channels);
// for(int i = 0; i < 3; i++){
//     getImageLayers(channels[i], layers);
//     for (int j = 0; j < 8; j++)
//     {
//         sprintf(filename, "../images/channels/bgr/%c/%d.jpg",labels[i],j);
//         imwrite(filename, layers[j]);
//     }
// }

// // GET HSV CHANNELS LAYERS
// Mat layers[8];
// Mat channels[3];
// char labels[] = "HSV";
// split(hsv, channels);
// for(int i = 0; i < 3; i++){
//     getImageLayers(channels[i], layers);
//     for (int j = 0; j < 8; j++)
//     {
//         sprintf(filename, "../images/channels/hsv/%c/%d.jpg",labels[i],j);
//         imwrite(filename, layers[j]);
//     }
// }