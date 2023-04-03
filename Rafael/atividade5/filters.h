#ifndef FILTERS
#define FILTERS
#define BLUR_SIZE 3
#define SOBEL_SIZE 3
#define ROBERTS_SIZE 2
#define PREWIT_SIZE 3
#define LAPLACIAN_SIZE 3

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

namespace filter
{
    int dir_x[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dir_y[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

    int blur[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobel_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    int roberts_x[3][3] = {{1, 0, 0}, {0, -1, 0}, {0, 0, 0}};
    int roberts_y[3][3] = {{0, 1, 0}, {-1, 0, 0}, {0, 0, 0}};

    int prewit_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int prewit_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    int laplacian[3][3] = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};

    bool inGrid(int i, int j, int lim_i, int lim_j)
    {
        return i >= 0 and j >= 0 and i < lim_i and j < lim_j;
    }

    void applyFilterPoint(cv::Mat &src, cv::Mat &dst, int i, int j, int mask[3][3])
    {
        int sum = 0;
        int cont = 0;
        for (int k = 0; k < 8; k++)
        {
            int y = i + dir_y[k];
            int x = j + dir_x[k];
            if (inGrid(y, x, src.rows, src.cols)){
                cont++;
                sum += src.at<uchar>(y, x) * mask[1 + dir_y[k]][1 + dir_x[k]];
            }
        }

        int mean = sum / cont;

        dst.at<uchar>(i, j) = mean;
    }

    void applyFilterGrid(cv::Mat &src, cv::Mat &dst, int mask[3][3], int n)
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