#include <opencv2/opencv.hpp>
#include <stdio.h>

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
                dst[bit].at<uchar>(i, j) = (src.at<uchar>(i, j) & mask) << 7 - bit;
            }
        }
    }
}

void getMaskFromHue(Mat &src, Mat &mask, int begin, int end, int begin_sat = 0, int end_sat = 255, int begin_val = 0, int end_val = 255)
{

    mask = Mat::zeros(src.size(), CV_8UC1);
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            uchar hue = src.at<Vec3b>(i, j)[0];
            uchar sat = src.at<Vec3b>(i, j)[1];
            uchar val = src.at<Vec3b>(i, j)[2];
            if (begin < hue and hue <= end and sat >= 0 and begin_sat <= sat and sat <= end_sat and begin_val <= val and val <= end_val)
                mask.at<uchar>(i, j) = 255;
        }
    }
}

void applyMaskBGR(Mat &src, Mat &dst, Mat &mask, bool remove = false, bool light = false)
{
    int reference = remove ? 255 : 0;
    dst = src.clone();
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            Vec3b pixel = src.at<Vec3b>(i, j);
            if (mask.at<uchar>(i, j) == reference)
            {
                for (int k = 0; k < 3; k++)
                    dst.at<Vec3b>(i, j)[k] = light * 255;
            }
        }
    }
}
int main()
{

    // SETTING VARIABLES
    char filename[100];
    std::string labels;
    Mat layers[8], channels[3];
    Mat gray, bgr, hsv, mask, ball;
    int F_ERODE, F_DILATE, S_ERODE;

    // RECEIVING IMAGE
    bgr = imread("../images/ball.jpg", IMREAD_COLOR);
    // GaussianBlur(bgr, bgr, Size(3, 3), 0, 0, BORDER_DEFAULT);

    // GET GRAY AND HSV
    cvtColor(bgr, gray, COLOR_BGR2GRAY);
    cvtColor(bgr, hsv, COLOR_BGR2HSV);

    // CREATE ERODE AND DILATE ELEMENTS
    Mat rec = getStructuringElement(0, Size(3, 3));
    Mat cross = getStructuringElement(1, Size(3, 3));
    Mat ellipse = getStructuringElement(1, Size(3, 3));

    {
        // GET BLUE BALL
        getMaskFromHue(hsv, mask, 100, 108);
        imwrite("../images/ball1/mask.jpg", mask);
        F_ERODE = 1, F_DILATE = 4, S_ERODE = 3;
        for (int i = 0; i < F_ERODE; i++)
            erode(mask, mask, rec);
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, rec);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, rec);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball1/ball.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
    }
    {
        // SET GRAY AND HSV AGAIN
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        // GET BASKETBALL BALL
        getMaskFromHue(hsv, mask, 10, 15);
        imwrite("../images/ball2/mask.jpg", mask);
        F_ERODE = 2, F_DILATE = 13, S_ERODE = 7;
        for (int i = 0; i < F_ERODE; i++)
        {
            erode(mask, mask, rec);
            // sprintf(filename, "../images/ball2/first-erode%d.jpg", i);
            // imwrite(filename, mask);
        }
        for (int i = 0; i < F_DILATE; i++)
        {
            dilate(mask, mask, ellipse);
            // sprintf(filename, "../images/ball2/first-dilate%d.jpg", i);
            // imwrite(filename, mask);
        }
        for (int i = 0; i < S_ERODE; i++)
        {
            erode(mask, mask, cross);
            // sprintf(filename, "../images/ball2/second-erode%d.jpg", i);
            // imwrite(filename, mask);
        }
        imwrite("../images/ball2/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball2/ball.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
    }
    {
        // SET GRAY AND HSV AGAIN
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        // GET FOOTBALL BALL
        getMaskFromHue(hsv, mask, 6, 9);
        imwrite("../images/ball3/mask.jpg", mask);
        F_ERODE = 2, F_DILATE = 15, S_ERODE = 13;
        for (int i = 0; i < F_ERODE; i++)
        {
            erode(mask, mask, ellipse);
            // sprintf(filename, "../images/ball3/first-erode%d.jpg", i);
            // imwrite(filename, mask);
        }
        for (int i = 0; i < F_DILATE; i++)
        {
            dilate(mask, mask, rec);
            // sprintf(filename, "../images/ball3/first-dilate%d.jpg", i);
            // imwrite(filename, mask);
        }
        for (int i = 0; i < S_ERODE; i++)
        {
            erode(mask, mask, cross);
            // sprintf(filename, "../images/ball3/second-erode%d.jpg", i);
            // imwrite(filename, mask);
        }
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball3/ball.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
    }
    {
        // SET GRAY AND HSV AGAIN
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        // GET FOOTBALL BALL
        getMaskFromHue(hsv, mask, 28, 40, 128, 256);
        imwrite("../images/ball4/mask.jpg", mask);
        F_ERODE = 2, F_DILATE = 15, S_ERODE = 12;
        for (int i = 0; i < F_DILATE; i++)
        {
            dilate(mask, mask, cross);
            // sprintf(filename, "../images/ball4/first-dilate%d.jpg", i);
            // imwrite(filename, mask);
        }
        for (int i = 0; i < S_ERODE; i++)
        {
            erode(mask, mask, ellipse);
            // sprintf(filename, "../images/ball4/second-erode%d.jpg", i);
            // imwrite(filename, mask);
        }
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball4/ball.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball5/ball.jpg", ball);
    }

    return 0;
}

// // GET BIT LAYERS:
// getImageLayers(gray, layers);
// for (int i = 0; i < 8; i++)
// {
//     sprintf(filename, "../images/layers/%d.jpg", i);
//     imwrite(filename, layers[i]);
// }

// // GET BGR CHANNELS
// labels = "BGR";
// split(bgr, channels);
// for (int i = 0; i < 3; i++)
// {
//     sprintf(filename, "../images/channels/bgr/%c.jpg", labels[i]);
//     imwrite(filename, channels[i]);
// }

// // GET HSV CHANNELS
// labels = "HSV";
// split(hsv, channels);
// for (int i = 0; i < 3; i++)
// {
//     sprintf(filename, "../images/channels/hsv/%c.jpg", labels[i]);
//     imwrite(filename, channels[i]);
// }

// // GET BGR CHANNELS LAYERS
// labels = "BGR";
// split(bgr, channels);
// for (int i = 0; i < 3; i++)
// {
//     getImageLayers(channels[i], layers);
//     for (int j = 0; j < 8; j++)
//     {
//         sprintf(filename, "../images/channels/bgr/%c/%d.jpg", labels[i], j);
//         imwrite(filename, layers[j]);
//     }
// }

// // GET HSV CHANNELS LAYERS
// labels = "HSV";
// split(hsv, channels);
// for (int i = 0; i < 3; i++)
// {
//     getImageLayers(channels[i], layers);
//     for (int j = 0; j < 8; j++)
//     {
//         sprintf(filename, "../images/channels/hsv/%c/%d.jpg", labels[i], j);
//         imwrite(filename, layers[j]);
//     }
// }
