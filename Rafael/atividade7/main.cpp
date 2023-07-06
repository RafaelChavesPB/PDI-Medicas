#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>

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

void getMaskFromHSV(Mat &src, Mat &mask, int begin, int end = 180, int begin_sat = 0, int end_sat = 255, int begin_val = 0, int end_val = 255)
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
    Mat gray, bgr, hsv, mask, aux, ball, circleImg;
    int F_ERODE, F_DILATE, S_ERODE;

    // RECEIVING IMAGE
    bgr = imread("../images/ball.jpg", IMREAD_COLOR);
    // GaussianBlur(bgr, bgr, Size(3, 3), 0, 0, BORDER_DEFAULT);

    // GET GRAY AND HSV
    cvtColor(bgr, gray, COLOR_BGR2GRAY);
    imwrite("../images/gray.jpg", gray);
    cvtColor(bgr, hsv, COLOR_BGR2HSV);
    imwrite("../images/hsv.jpg", hsv);

    // CREATE ERODE AND DILATE ELEMENTS
    Mat rec = getStructuringElement(0, Size(3, 3));
    Mat cross = getStructuringElement(1, Size(3, 3));
    Mat ellipse = getStructuringElement(1, Size(3, 3));

    // GET BLUE BALL
    {
        getMaskFromHSV(hsv, mask, 100, 108);
        imwrite("../images/ball1/initial_mask.jpg", mask);
        F_ERODE = 1, F_DILATE = 4, S_ERODE = 3;
        for (int i = 0; i < F_ERODE; i++)
            erode(mask, mask, rec);
        imwrite("../images/ball1/first-erode.jpg", mask);
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, rec);
        imwrite("../images/ball1/first-dilate.jpg", mask);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, rec);
        imwrite("../images/ball1/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball1/ball1.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball1/balls.jpg", bgr);
    }
    // GET BASKETBALL BALL
    {
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        getMaskFromHSV(hsv, mask, 10, 15);
        imwrite("../images/ball2/initial_mask.jpg", mask);
        F_ERODE = 2, F_DILATE = 13, S_ERODE = 7;
        for (int i = 0; i < F_ERODE; i++)
            erode(mask, mask, rec);
        imwrite("../images/ball2/first-erode.jpg", mask);
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, ellipse);
        imwrite("../images/ball2/first-dilate.jpg", mask);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, cross);
        imwrite("../images/ball2/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball2/ball2.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball2/balls.jpg", bgr);
    }
    // GET FUTEBOL AMERICANO BALL
    {

        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        getMaskFromHSV(hsv, mask, 6, 9);
        imwrite("../images/ball3/initial_mask.jpg", mask);
        F_ERODE = 2, F_DILATE = 15, S_ERODE = 13;
        for (int i = 0; i < F_ERODE; i++)
            erode(mask, mask, ellipse);
        imwrite("../images/ball3/first-erode.jpg", mask);
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, rec);
        imwrite("../images/ball3/first-dilate.jpg", mask);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, cross);
        imwrite("../images/ball3/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball3/ball3.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball3/balls.jpg", bgr);
    }
    // GET TENNIS BALL
    {

        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        getMaskFromHSV(hsv, mask, 28, 40, 128);
        imwrite("../images/ball4/initial_mask.jpg", mask);
        F_DILATE = 15, S_ERODE = 12;
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, cross);
        imwrite("../images/ball4/first-dilate.jpg", mask);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, ellipse);
        imwrite("../images/ball4/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball4/ball4.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball4/balls.jpg", bgr);
    }
    // GET SINUCA BALL
    {

        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        getMaskFromHSV(hsv, mask, 20, 25, 180);
        imwrite("../images/ball5/initial_mask.jpg", mask);
        F_DILATE = 25, S_ERODE = 22;
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, ellipse);
        imwrite("../images/ball5/first-dilate.jpg", mask);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, ellipse);
        imwrite("../images/ball5/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball5/ball5.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball5/balls.jpg", bgr);
    }
    // GET HOCKEY BALL
    {

        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        getMaskFromHSV(hsv, mask, 15, 40, 10, 80);
        imwrite("../images/ball6/initial_mask.jpg", mask);
        F_ERODE = 3, F_DILATE = 8, S_ERODE = 0;
        for (int i = 0; i < F_ERODE; i++)
            erode(mask, mask, rec);
        imwrite("../images/ball6/first-erode.jpg", mask);
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, ellipse);
        imwrite("../images/ball6/first-dilate.jpg", mask);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, ellipse);
        imwrite("../images/ball6/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball6/ball6.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball6/balls.jpg", bgr);
    }
    // GET PETECA BALL
    {

        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        getMaskFromHSV(hsv, mask, 0, 50);
        imwrite("../images/ball7/initial_mask.jpg", mask);
        F_ERODE = 9, F_DILATE = 20, S_ERODE = 8;
        for (int i = 0; i < F_ERODE; i++)
            erode(mask, mask, cross);
        imwrite("../images/ball7/first-erode.jpg", mask);
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, rec);
        imwrite("../images/ball7/first-dilate.jpg", mask);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, cross);
        imwrite("../images/ball7/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball7/ball7.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball7/balls.jpg", bgr);
    }
    // GET BOCHA BALL
    {

        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        getMaskFromHSV(hsv, aux, 20, 50, 15);
        getMaskFromHSV(hsv, mask, 175, 180, 15);
        mask |= aux;
        imwrite("../images/ball8/initial_mask.jpg", mask);
        F_ERODE = 1, F_DILATE = 10, S_ERODE = 9;
        for (int i = 0; i < F_ERODE; i++)
            erode(mask, mask, cross);
        imwrite("../images/ball8/first-erode.jpg", mask);
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, rec);
        imwrite("../images/ball8/first-dilate.jpg", mask);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, ellipse);
        imwrite("../images/ball8/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball8/ball8.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball8/balls.jpg", bgr);
    }
    // GET CIRCLES
    {
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        circleImg = Mat::zeros(bgr.size(), CV_8UC3);
        std::vector<Vec3f> circles;
        Scalar colors[4] = {Scalar(0, 21, 255), Scalar(0, 170, 255), Scalar(255, 0, 0), Scalar(255, 0, 255)};
        HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / 16, 100, 30, 30, 100);
        for (size_t i = 0; i < 4; i++)
        {
            Vec3i c = circles[i];
            Point center = Point(c[0], c[1]);
            circle(circleImg, center, 2, colors[i], 3);
            int radius = c[2];
            circle(circleImg, center, radius, colors[i], 3);
        }
        imwrite("../images/circles.jpg", circleImg);
        cvtColor(circleImg, circleImg, COLOR_BGR2HSV);
    }
    // GET FUTEBOL BALL
    {
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        getMaskFromHSV(hsv, mask, 115, 120, 5, 10);
        // getMaskFromHSV(circleImg, aux, 90, 120);
        // mask |= aux;
        imwrite("../images/ball9/initial_mask.jpg", mask);
        F_ERODE = 1, F_DILATE = 40, S_ERODE = 15;
        for (int i = 0; i < F_ERODE; i++)
            erode(mask, mask, rec);
        imwrite("../images/ball9/first-erode.jpg", mask);
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, rec);
        imwrite("../images/ball9/first-dilate.jpg", mask);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, ellipse);
        imwrite("../images/ball9/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball9/ball9.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball9/balls.jpg", bgr);
    }
    // GET BEISEBOL BALL
    {
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        getMaskFromHSV(hsv, mask, 0, 15);
        // getMaskFromHSV(circleImg, aux, 120, 150);
        // mask |= aux;
        imwrite("../images/ball10/initial_mask.jpg", mask);
        F_ERODE = 1, F_DILATE = 32, S_ERODE = 15;
        for (int i = 0; i < F_ERODE; i++)
            erode(mask, mask, rec);
        imwrite("../images/ball10/first-erode.jpg", mask);
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, rec);
        imwrite("../images/ball10/first-dilate.jpg", mask);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, ellipse);
        imwrite("../images/ball10/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball10/ball10.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball10/balls.jpg", bgr);
    }
    // GET VOLEI BALL
    {
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        getMaskFromHSV(hsv, mask, -1, 180, 0, 255, 0, 190);
        // getMaskFromHSV(circleImg, aux, 0, 15);
        // mask |= aux;
        imwrite("../images/ball11/initial_mask.jpg", mask);
        F_ERODE = 0, F_DILATE = 30, S_ERODE = 15;
        for (int i = 0; i < F_ERODE; i++)
            erode(mask, mask, rec);
        imwrite("../images/ball11/first-erode.jpg", mask);
        for (int i = 0; i < F_DILATE; i++)
            dilate(mask, mask, rec);
        imwrite("../images/ball11/first-dilate.jpg", mask);
        for (int i = 0; i < S_ERODE; i++)
            erode(mask, mask, ellipse);
        imwrite("../images/ball11/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball11/ball11.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        imwrite("../images/ball12/ball12.jpg", bgr);
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
