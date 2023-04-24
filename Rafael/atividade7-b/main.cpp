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

    // BOLE FUTEBOL AMERICANO
    // 128 - Basquete total, tenis total, futebol americano total, volei total.
    // 64 - futebol americano bem - volei e basquete faltando partes
    // 32 - partes mais escuras da bola de futebol
    // 0 - 64, 0 - 90, faltam partes mas da pra erodir e expandir
    // r 0 - 128 - não tem nada que possa usar mas posso eliminar
    // aux = imread("../images/channels/hsv/S/7.jpg", IMREAD_GRAYSCALE);
    getMaskFromHSV(bgr, mask, 0, 64, 32, 90,  135);
    imwrite("../images/ball1/mask0.jpg", mask);
    std::vector<int> iteractionsBall1 = {1, 10, 10, 15, 5};
    for(int i = 0; i < iteractionsBall1.size(); i++){
        if(i&1)
            dilate(mask, mask, rec, Point(-1,-1), iteractionsBall1[i]);
        else
            erode(mask, mask, rec, Point(-1,-1), iteractionsBall1[i]);
        imwrite(std::string("../images/ball1/mask") + std::to_string(i+1) + std::string(".jpg"), mask);
    }
    applyMaskBGR(bgr, ball, mask, false, true);
    imwrite("../images/ball1/ball-a.jpg", ball);
    applyMaskBGR(bgr, ball, mask);
    imwrite("../images/ball1/ball-b.jpg", ball);
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
