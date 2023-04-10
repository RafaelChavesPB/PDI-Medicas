#ifndef ALGO
#define ALGO
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <utility>
#define HSV_COLOR_LIM 180
#define BL std::cout << std::endl;
#define NL std::endl;

bool rec(int histogram[], int it, int curr_min, int curr_max, int min, int max, int q_buckets, int n)
{
    if (q_buckets == n)
        return it == HSV_COLOR_LIM;

    int acc = 0;
    bool answer = false;
    for (int i = it; i < HSV_COLOR_LIM; i++)
    {
        acc += histogram[i];
        if (acc >= min)
        {
            if (acc > max)
                break;
            answer = answer or rec(histogram, i + 1, std::min(acc, curr_min), std::max(acc, curr_max), min, max, q_buckets + 1, n);
        }
    }
    return answer;
}

std::pair<int, std::vector<int>> rec(int histogram[], int it, int curr_min, int curr_max, int min, int max, int q_buckets, int n, std::vector<int> buckets)
{
    if (q_buckets == n)
    {
        if (it == HSV_COLOR_LIM)
        {
            // for (int i = 0; i < n; i++)
            //     std::cout << buckets[i] << " ";
            // BL;
            return {curr_max - curr_min, buckets};
        }
        return {INT_MAX, std::vector<int>()};
    }

    std::pair<int, std::vector<int>> answer = {INT_MAX, std::vector<int>()};
    int acc = 0;
    for (int i = it; i < HSV_COLOR_LIM; i++)
    {
        acc += histogram[i];
        if (acc >= min)
        {
            if (acc > max)
                break;
            buckets.push_back(i + 1);
            std::pair<int, std::vector<int>> curr = rec(histogram, i + 1, std::min(acc, curr_min), std::max(acc, curr_max), min, max, q_buckets + 1, n, buckets);
            if (curr.first < answer.first)
                answer = curr;
            buckets.pop_back();
        }
    }
    return answer;
}

int find_buckets(int histogram[], int buckets[], int n)
{
    int mean = 0;
    for (int i = 0; i < HSV_COLOR_LIM; i++)
        mean += histogram[i];
    mean /= n;

    int begin = 0, end = 0, mid = 0, max, min;
    while (true)
    {
        max = mean + end;
        min = mean - end;
        if (rec(histogram, 0, mean, 0, min, max, 0, n))
            break;

        begin = end;
        end = 2 * end + 1;
    }
    while (begin < end)
    {
        mid = (begin + end) / 2;
        max = mean + mid;
        min = mean - mid;
        bool flag = rec(histogram, 0, mean, 0, min, max, 0, n);
        if (flag)
            end = mid;
        else
            begin = mid + 1;
    }
    max = mean + end;
    min = mean - end;
    auto answer = rec(histogram, 0, mean, 0, min, max, 0, n, std::vector<int>());
    std::cout << "Mean: " << mean << " Min: " << min << " Max: " << max << " Max Dist: " << answer.first << NL;
    BL;
    for (int i = 0; i < n; i++)
        buckets[i] = answer.second[i];
    return answer.first;
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
                    if(hue < buckets[c]){
                        hsv.at<cv::Vec3b>(i, j)[0] = centroids[c];
                        hsv.at<cv::Vec3b>(i, j)[1] = 128;
                        hsv.at<cv::Vec3b>(i, j)[2] = 128;
                        freq[c]++;
                        break;
                    }
            }
        }

        std::cout << "Centroids: " << std::endl;
        for (int i = 0; i < n; i++)
        {
            std::cout << centroids[i] << " - "<< buckets[i] << " - " << freq[i] << NL;
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