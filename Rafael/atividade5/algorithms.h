#ifndef ALGO
#define ALGO
#include <opencv2/opencv.hpp>
#include <iostream>
#define HSV_COLOR_LIM 256
#define BL std::cout << std::endl;
#define NL std::endl;

bool rec(int histogram[], int it, int curr_min, int curr_max, int min, int max, int q_buckets, int n)
{
    if (it == n)
        return q_buckets == n;

    int acc = 0;
    bool answer = false;
    for (int i = it; i < HSV_COLOR_LIM; i++)
    {
        acc += histogram[i];
        if (acc >= min)
        {
            if (acc > max)
                break;
            answer = answer or rec(histogram, it + 1, std::min(acc, curr_min), std::max(acc, curr_max), min, max, q_buckets + 1, n);
        }
    }
    return answer;
}


bool find_buckets(int histogram[], int buckets[], int n)
{
    int mean = 0;
    for (int i = 0; i < HSV_COLOR_LIM; i++)
        mean += histogram[i];
    mean /= n;

    std::cout << mean << NL;

    int begin = 0, end = 0, mid = 0, max, min;
    std::cout << "ENDS:" << NL while (true)
    {
        max = mean + end;
        min = mean - end;
        if (rec(histogram, 0, mean, 0, min, max, 0, n) /*calc*/)
            break;
        
        begin = end;
        end = 2 * end + 1;
        std::cout << end << NL
    }
    BL;
    std::cout << "MIDS:" << NL while (begin <= end)
    {
        mid = (begin + end) / 2;
        max = mean + mid;
        min = mean - mid;
        bool flag = rec(histogram, 0, mean, 0, min, max, 0, n); //calc
        std::cout << begin << " " << mid << " " << end << " " << flag << NL;
        if (flag)
            end = mid - 1;
        else
            begin = mid + 1;
    }
    BL;
    return rec(histogram, 0, mean, 0, min, max, 0, n); //calc
}

namespace algo
{
    void kmeans(cv::Mat &hsv, int n)
    {
        int buckets[n] = {0};
        long buckets_amount[n] = {0};
        int histogram[HSV_COLOR_LIM] = {0};
        for (int i = 0; i < hsv.cols; i++)
        {
            for (int j = 0; j < hsv.rows; j++)
            {
                int color = hsv.at<cv::Vec3b>(i, j)[2];
                histogram[color]++;
            }
        }

        std::cout <<  find_buckets(histogram, buckets, n)  << NL;
        // std::cout << "Pixels: " << hsv.cols * hsv.rows << NL;
        // BL;
        // for (int i = 0, b = 0; i < HSV_COLOR_LIM; i++)
        // {
        //     if (i == buckets[b])
        //         b++;
        //     buckets_amount[b] += histogram[i];
        // }
        // std::cout << "Buckets" << NL;
        // for (int i = 0; i < n; i++)
        //     std::cout << buckets[i] << " - " << buckets_amount[i] << NL;
        // BL;
        std::cout << "Frequency" << NL;
        int acc = 0;
        for (int i = 0, b = 0; i < HSV_COLOR_LIM; i++){
            acc += histogram[i];
            std::cout << i << ": " << histogram[i] << " " << acc << NL;
        }
    }
}

#endif