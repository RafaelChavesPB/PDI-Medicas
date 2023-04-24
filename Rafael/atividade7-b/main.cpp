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
                dst[bit].at<uchar>(i, j) = ((src.at<uchar>(i, j) & mask) >> bit) * 255;
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
    Mat gray, bgr, hsv, mask, aux, ball;

    // RECEIVING IMAGE
    bgr = imread("../images/balls.png", IMREAD_COLOR);

    // GET GRAY AND HSV
    cvtColor(bgr, gray, COLOR_BGR2GRAY);
    imwrite("../images/gray.jpg", gray);
    cvtColor(bgr, hsv, COLOR_BGR2HSV);
    imwrite("../images/hsv.jpg", hsv);

    // CREATE ERODE AND DILATE ELEMENTS
    Mat rec = getStructuringElement(0, Size(3, 3));
    Mat cross = getStructuringElement(1, Size(3, 3));
    Mat ellipse = getStructuringElement(1, Size(5, 5));

    // BOLA FUTEBOL AMERICANO
    {
        getMaskFromHSV(bgr, mask, 0, 64, 32, 90, 135);
        imwrite("../images/ball1/mask0.jpg", mask);
        std::vector<int> iteractionsBall1 = {1, 10, 10, 16, 5};
        for (int i = 0; i < iteractionsBall1.size(); i++)
        {
            if (i & 1)
                dilate(mask, mask, rec, Point(-1, -1), iteractionsBall1[i]);
            else
                erode(mask, mask, rec, Point(-1, -1), iteractionsBall1[i]);
            imwrite(std::string("../images/ball1/mask") + std::to_string(i + 1) + std::string(".jpg"), mask);
        }
        imwrite("../images/ball1/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball1/ball-a.jpg", ball);
        applyMaskBGR(bgr, ball, mask);
        imwrite("../images/ball1/ball-b.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        imwrite("../images/ball2/balls.jpg", bgr);
    }

    // BOLA BASQUETE
    {
        getMaskFromHSV(hsv, mask, 7, 11, 150);
        imwrite("../images/ball2/mask0.jpg", mask);
        std::vector<int> iteractionsBall2 = {0, 3};
        for (int i = 0; i < iteractionsBall2.size(); i++)
        {
            if (i & 1)
                dilate(mask, mask, rec, Point(-1, -1), iteractionsBall2[i]);
            else
                erode(mask, mask, rec, Point(-1, -1), iteractionsBall2[i]);
            imwrite(std::string("../images/ball2/mask") + std::to_string(i + 1) + std::string(".jpg"), mask);
        }
        imwrite("../images/ball2/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball2/ball-a.jpg", ball);
        applyMaskBGR(bgr, ball, mask);
        imwrite("../images/ball2/ball-b.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        imwrite("../images/ball3/balls.jpg", bgr);
    }

    // BOLA TENNIS
    {
        getMaskFromHSV(hsv, mask, 39, 40, 128);
        imwrite("../images/ball3/mask0.jpg", mask);
        std::vector<int> iteractionsBall3 = {0, 8};
        for (int i = 0; i < iteractionsBall3.size(); i++)
        {
            if (i & 1)
                dilate(mask, mask, rec, Point(-1, -1), iteractionsBall3[i]);
            else
                erode(mask, mask, rec, Point(-1, -1), iteractionsBall3[i]);
            imwrite(std::string("../images/ball3/mask") + std::to_string(i + 1) + std::string(".jpg"), mask);
        }
        imwrite("../images/ball3/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball3/ball-a.jpg", ball);
        applyMaskBGR(bgr, ball, mask);
        imwrite("../images/ball3/ball-b.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        imwrite("../images/ball4/balls.jpg", bgr);
    }

    // BOLA VOLEI
    {
        getMaskFromHSV(hsv, mask, 12, 33, 128);
        imwrite("../images/ball4/mask0.jpg", mask);
        std::vector<int> iteractionsBall4 = {0, 3};
        for (int i = 0; i < iteractionsBall4.size(); i++)
        {
            if (i & 1)
                dilate(mask, mask, rec, Point(-1, -1), iteractionsBall4[i]);
            else
                erode(mask, mask, rec, Point(-1, -1), iteractionsBall4[i]);
            imwrite(std::string("../images/ball4/mask") + std::to_string(i + 1) + std::string(".jpg"), mask);
        }
        imwrite("../images/ball4/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball4/ball-a.jpg", ball);
        applyMaskBGR(bgr, ball, mask);
        imwrite("../images/ball4/ball-b.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        imwrite("../images/ball5/balls.jpg", bgr);
    }

    // BOLA BEISEBALL
    {
        getMaskFromHSV(hsv, mask, 0, 90, 60, 255, 0, 228);
        imwrite("../images/ball5/mask0.jpg", mask);
        std::vector<int> iteractionsBall5 = {0, 30, 22};
        for (int i = 0; i < iteractionsBall5.size(); i++)
        {
            if (i & 1)
                dilate(mask, mask, rec, Point(-1, -1), iteractionsBall5[i]);
            else
                erode(mask, mask, rec, Point(-1, -1), iteractionsBall5[i]);
            imwrite(std::string("../images/ball5/mask") + std::to_string(i + 1) + std::string(".jpg"), mask);
        }
        imwrite("../images/ball5/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball5/ball-a.jpg", ball);
        applyMaskBGR(bgr, ball, mask);
        imwrite("../images/ball5/ball-b.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
        imwrite("../images/ball6/balls.jpg", bgr);
    }
    // BOLA BEISEBALL
    {
        getMaskFromHSV(hsv, mask, 90, 180, 10, 255);
        imwrite("../images/ball6/mask0.jpg", mask);
        std::vector<int> iteractionsBall6 = {0, 15};
        for (int i = 0; i < iteractionsBall6.size(); i++)
        {
            if (i & 1)
                dilate(mask, mask, rec, Point(-1, -1), iteractionsBall6[i]);
            else
                erode(mask, mask, rec, Point(-1, -1), iteractionsBall6[i]);
            imwrite(std::string("../images/ball6/mask") + std::to_string(i + 1) + std::string(".jpg"), mask);
        }
        imwrite("../images/ball6/mask.jpg", mask);
        applyMaskBGR(bgr, ball, mask, false, true);
        imwrite("../images/ball6/ball-a.jpg", ball);
        applyMaskBGR(bgr, ball, mask);
        imwrite("../images/ball6/ball-b.jpg", ball);
        applyMaskBGR(bgr, bgr, mask, true, true);
        cvtColor(bgr, gray, COLOR_BGR2GRAY);
        cvtColor(bgr, hsv, COLOR_BGR2HSV);
    }
    // AUXILIAR
    {
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
    }

    return 0;
}
