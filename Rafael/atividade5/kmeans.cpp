#include <opencv2/opencv.hpp>
#include <iostream>
#include "main.h"
#include "filters.h"
#include "algorithms.h"
#include "masks.h"
#include <map>

int main(int argc, char **argv)
{
    std::string filename, image_name, prefix("../generated/");
    int ds = std::stoi(argv[argc - 1]);
    for (int i = 1; i < argc - 1; i++)
    {
        filename = std::string(argv[i]);
        image_name = treat::getImageName(filename);
        system((std::string("rm -r ") + prefix + image_name).c_str());
        system((std::string("mkdir ") + prefix + image_name).c_str());
        cv::Mat src = cv::imread(filename), dst;
        // cv::Vec3b centroids[ds], buckets[ds];
        // algo::kmeans(src, buckets, centroids, ds);
        // cv::imwrite(prefix + image_name + std::string("/kmeans.jpg"), dst);
        std::map<int, int> freq;
        cv::Vec3i pixel;
        std::vector<std::vector<char>> grid(src.rows, std::vector<char>(src.cols));

        return 0;
    }
}