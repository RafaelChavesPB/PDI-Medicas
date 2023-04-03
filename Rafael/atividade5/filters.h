#ifndef FILTERS
#define FILTERS

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

namespace edge
{
    
    int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobel_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
#define SOBEL_SIZE 3

    int roberts_x[3][3] = {{1, 0, 0}, {0, -1, 0}, {0, 0, 0}};
    int roberts_y[3][3] = {{0, 1, 0}, {-1, 0, 0}, {0, 0, 0}};
#define ROBERTS_SIZE 2

    int prewit_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int prewit_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
#define PREWIT_SIZE 3

    int laplacian[3][3] = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
#define LAPLACIAN_SIZE 3

    void applyFilterSection(cv::Mat &src, cv::Mat &dst, int y, int x, int mask[3][3], int n_y, int n_x)
    {
        int sum = 0;
        int cont = 0;
        for (int i = 0; i < n_y; i++)
        {
            for (int j = 0; j < n_x; j++)
            {
                cont++;
                sum += src.at<uchar>(y + i, x + j) * mask[i][j];
            }
        }

        int mean = sum / cont;
        for (int i = y; i < y + n_y; i++)
        {
            for (int j = x; j < x + n_x; j++)
            {
                dst.at<uchar>(i, j) = mean;
            }
        }


    }

    void applyFilterGrid(cv::Mat &src, cv::Mat &dst, int mask[3][3], int n)
    {
        // It does what you think its does
        dst = src.clone();
        for (int i = 0; i < dst.rows; i++)
        {
            for (int j = 0; j < dst.cols; j++)
            {
                applyFilterSection(src, dst, i, j, mask, std::min(n, dst.rows - i), std::min(n, dst.cols - j));
            }
        }
    }
}

#endif