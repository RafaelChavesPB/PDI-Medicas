#ifndef ALGO
#define ALGO
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <utility>
#define HSV_COLOR_LIM 180
#define LIM 256
#define BL std::cout << std::endl;
#define NL std::endl;

std::pair<bool, std::vector<int>> greedy(int histogram[], int min, int max, int n)
{
    int acc = 0;
    std::vector<int> buckets;
    for (int i = 0; i < LIM; i++)
    {
        if (min <= acc)
        {
            if (acc + histogram[i] <= max)
                acc += histogram[i];
            else
            {
                acc = histogram[i];
                buckets.push_back(i);
            }
            continue;
        }

        if (acc + histogram[i] <= max)
            acc += histogram[i];

        else
            return {false, std::vector<int>()};
    }
    return {buckets.size() == n, buckets};
}

int find_buckets(int histogram[], int buckets[], int n)
{
    int total = 0;
    for (int i = 0; i < HSV_COLOR_LIM; i++)
        total += histogram[i];
    int mean = total /= n;

    int begin = 0, end = 0, mid = 0, max, min;
    std::pair<bool, std::vector<int>> ans;
    while (begin < end)
    {
        mid = (begin + end) / 2;
        max = mean + mid;
        min = mean - mid;
        ans = greedy(histogram, min, max, n);
        if (ans.first)
            end = mid;
        else
            begin = mid + 1;
    }
    max = mean + end;
    min = mean - end;
    for (int i = 0; i < n; i++)
        buckets[i] = ans.second[i];
    return ans.first;
}

void find_centroids(int histogram[], int buckets[], int centroids[], int n)
{
    int begin, end = 0;
    for (int b = 0; b < n; b++)
    {
        begin = end;
        end = buckets[b];
        std::pair<long, int> best = {LONG_MAX, begin};
        for (int i = begin; i < end; i++)
        {
            long dist = 0;
            for (int j = begin; j < end; j++)
                dist += abs(i - j) * histogram[j];
            best = std::min({dist, i}, best);
        }
        centroids[b] = best.second;
    }
}

namespace algo
{
    void kmeans(cv::Mat &hsv, int buckets[], int centroids[], int n)
    {
        int freq[n] = {0};
        long buckets_amount[n] = {0};
        int histogram[HSV_COLOR_LIM] = {0};
        int new_histogram[HSV_COLOR_LIM] = {0};
        for (int i = 0; i < hsv.rows; i++)
        {
            for (int j = 0; j < hsv.cols; j++)
            {
                int hue = hsv.at<cv::Vec3b>(i, j)[0];
                histogram[hue]++;
            }
        }

        find_buckets(histogram, buckets, n);
        find_centroids(histogram, buckets, centroids, n);

        for (int i = 0; i < hsv.rows; i++)
        {
            for (int j = 0; j < hsv.cols; j++)
            {
                int hue = hsv.at<cv::Vec3b>(i, j)[0];
                for (int c = 0; c < n; c++)
                    if (hue < buckets[c])
                    {
                        hsv.at<cv::Vec3b>(i, j)[0] = centroids[c];
                        freq[c]++;
                        break;
                    }
            }
        }

        std::cout << "Centroids: " << std::endl;
        for (int i = 0; i < n; i++)
        {
            std::cout << centroids[i] << " - " << buckets[i] << " - " << freq[i] << NL;
        }
        BL;
        BL;

        std::cout << "Pixels: " << hsv.cols * hsv.rows << NL;
        BL;

        for (int i = 0; i < hsv.rows; i++)
        {
            for (int j = 0; j < hsv.cols; j++)
            {
                int hue = hsv.at<cv::Vec3b>(i, j)[0];
                new_histogram[hue]++;
            }
        }

        std::cout << "Histograms: " << std::endl;
        int acc = 0;
        for (int i = 0; i < HSV_COLOR_LIM; i++)
        {
            acc += histogram[i];
            std::cout << i << ": " << histogram[i] << " " << acc << " " << NL;
        }

        std::cout << "New Histograms: " << std::endl;
        int new_acc = 0;
        for (int i = 0; i < HSV_COLOR_LIM; i++)
        {
            new_acc += new_histogram[i];
            std::cout << i << ": " << new_histogram[i] << " " << new_acc << " " << NL;
        }
    }
}

#endif