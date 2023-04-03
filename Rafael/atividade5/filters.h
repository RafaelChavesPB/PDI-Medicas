#ifndef FILTERS
#define FILTERS
#define TRHESHOLD 5

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

namespace filter
{
    int dir_x[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dir_y[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

    int sobel_x[3][3] = {{-1, 0, 1},
                         {-2, 0, 2},
                         {-1, 0, 1}};
    int sobel_y[3][3] = {{1, 2, 1},
                         {0, 0, 0},
                         {-1, -2, -1}};

    int roberts[3][3] = {{0, 1, 0},
                         {-1, 0, 0},
                         {0, 0, 0}};

    int prewit_x[3][3] = {{-1, 0, 1},
                          {-1, 0, 1},
                          {-1, 0, 1}};
    int prewit_y[3][3] = {{-1, -1, -1},
                          {0, 0, 0},
                          {1, 1, 1}};

    int laplacian[3][3] = {{0, 1, 0},
                           {1, -4, 1},
                           {0, 1, 0}};

    int laplacian_[3][3] = {{1, 1, 1},
                            {1, -8, 1},
                            {1, 1, 1}};

    bool inGrid(int i, int j, int lim_i, int lim_j)
    {
        return i >= 0 and j >= 0 and i < lim_i and j < lim_j;
    }

    void applyBlurPoint(cv::Mat &src, cv::Mat &dst, int i, int j, int window_size)
    {
        int sum = 0;
        int cont = 0;
        for (int di = -window_size / 2; di <= window_size / 2; di++)
        {
            for (int dj = -window_size / 2; dj <= window_size / 2; dj++)
            {
                int y = i + di;
                int x = j + dj;
                if (inGrid(y, x, src.rows, src.cols))
                {
                    cont++;
                    sum += src.at<uchar>(y, x);
                }
            }
        }
        dst.at<uchar>(i, j) = sum / cont;
    }

    void applyBlurGrid(cv::Mat &src, cv::Mat &dst, int window_size)
    {
        // It does what you think its does
        if (window_size % 2 == 0)
            window_size++;

        dst = src.clone();
        for (int i = 0; i < dst.rows; i++)
            for (int j = 0; j < dst.cols; j++)
                applyBlurPoint(src, dst, i, j, window_size);
    }

    void applyFilterPoint(cv::Mat &src, cv::Mat &dst, int i, int j, int mask[3][3])
    {
        int sum = 0;
        int cont = 0;
        for (int k = 0; k < 8; k++)
        {
            int y = i + dir_y[k];
            int x = j + dir_x[k];
            if (inGrid(y, x, src.rows, src.cols))
            {
                cont++;
                sum += src.at<uchar>(y, x) * mask[1 + dir_y[k]][1 + dir_x[k]];
            }
        }

        int mean = abs(sum);

        dst.at<uchar>(i, j) = mean;
    }

    void applyFilterGrid(cv::Mat &src, cv::Mat &dst, int mask[3][3])
    {
        // It does what you think its does
        dst = src.clone();
        for (int i = 0; i < dst.rows; i++)
        {
            for (int j = 0; j < dst.cols; j++)
            {
                applyFilterPoint(src, dst, i, j, mask);
            }
        }
    }
}

#endif