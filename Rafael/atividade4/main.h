#include <opencv2/opencv.hpp>
#include <utility>

namespace pdi
{

    void binarize(cv::Mat &src, cv::Mat &dst, int threshold)
    {
        src.copyTo(dst);
        for (int i = 0; i < dst.rows; i++)
        {
            for (int j = 0; j < dst.cols; j++)
            {
                dst.at<uchar>(i, j) = dst.at<uchar>(i, j) >= threshold ? 255 : 0;
            }
        }
    }

    void binarize_inv(cv::Mat &src, cv::Mat &dst, int threshold)
    {
        src.copyTo(dst);
        for (int i = 0; i < dst.rows; i++)
        {
            for (int j = 0; j < dst.cols; j++)
            {
                dst.at<uchar>(i, j) = dst.at<uchar>(i, j) >= threshold ? 0 : 255;
            }
        }
    }

    int changePixelGamma(uchar pixel, double gamma)
    {
        const double max_intensity = 255;
        const double pixel_normalized = static_cast<int>(pixel) / max_intensity;
        return max_intensity * pow(pixel_normalized, gamma);
    }

    void changeImageGamma(cv::Mat &src, cv::Mat &dst, double gamma)
    {
        src.copyTo(dst);
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols; j++)
            {
                dst.at<uchar>(i, j) = changePixelGamma(src.at<uchar>(i, j), gamma);
            }
        }
    }

    void window_filter(cv::Mat &src, cv::Mat &dst, int lower, int upper)
    {
        src.copyTo(dst);
        for (int i = 0; i < dst.rows; i++)
        {
            for (int j = 0; j < dst.cols; j++)
            {
                uchar pixel = dst.at<uchar>(i, j);
                if (pixel > upper or pixel < lower)
                    dst.at<uchar>(i, j) = 255;
            }
        }
    }

    void getImageLayers(cv::Mat &src, cv::Mat *dst)
    {
        for (int bit = 0; bit < 8; bit++)
        {
            src.copyTo(dst[bit]);
            int mask = 1 << bit;
            for (int i = 0; i < src.rows; i++)
            {
                for (int j = 0; j < src.cols; j++)
                {
                    dst[bit].at<uchar>(i, j) = src.at<uchar>(i, j) & mask;
                }
            }
        }
    }

    void joinImageLayers(cv::Mat *src, cv::Mat &dst, int mymask)
    {
        dst = cv::Mat::zeros(src[0].size(), src[0].type());
        for (int bit = 0; bit < 8; bit++)
        {
            int mask = 1 << bit;
            if (mask & mymask)
            {
                for (int i = 0; i < dst.rows; i++)
                {
                    for (int j = 0; j < dst.cols; j++)
                    {
                        dst.at<uchar>(i, j) |= src[bit].at<uchar>(i, j);
                    }
                }
            }
        }
    }

    int BGRtoGrayMean(cv::Vec3b pixel)
    {
        return (pixel[2] + pixel[1] + pixel[0]) / 3;
    }

    int BGRtoGrayWeighted(cv::Vec3b pixel)
    {
        return 0.2989 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0];
    }

    void convToGray(cv::Mat &src, cv::Mat &dst, int (*func)(cv::Vec3b))
    {
        dst = cv::Mat::zeros(src.size(), CV_8UC1);
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols; j++)
            {
                cv::Vec3b pixel = src.at<cv::Vec3b>(i, j);
                dst.at<uchar>(i, j) = static_cast<uchar>(func(pixel));
            }
        }
    }

    void filterHue(cv::Mat &src, cv::Mat *dst, std::pair<int, int> *ranges)
    {
        for (int k = 0; k < sizeof(ranges) / sizeof(int); k++)
        {
            dst[k] = cv::Mat::zeros(src.size(), CV_8UC1);
            for (int i = 0; i < src.rows; i++)
            {
                for (int j = 0; j < src.cols; j++)
                {
                    uchar hue = src.at<cv::Vec3b>(i, j)[0];
                    dst[k].at<uchar>(i, j) = ranges[k].first < hue and hue <= ranges[k].first ? 255 : 0;
                }
            }
        }
    }

    void getColorRange(cv::Mat &src, cv::Mat &dst, int start, int end)
    {
        dst = src.clone();
        for (int i = 0; i < dst.rows; i++)
        {
            for (int j = 0; j < dst.cols; j++)
            {
                uchar color = dst.at<cv::Vec3b>(i, j)[0];
                uchar instensity = dst.at<cv::Vec3b>(i, j)[2];
                dst.at<cv::Vec3b>(i, j)[2] = start <= color and color < end ? instensity : 0;
            }
        }
    }
}

namespace treat
{
    std::string getImageName(std::string filename)
    {
        int start = 0, end;
        for (int i = 0; i < filename.size(); i++)
        {
            if (filename[i] == '.')
            {
                end = i;
            }
        }
        for (int r = end; r >= 0; r--)
        {
            if (filename[r] == '/')
            {
                start = r + 1;
                break;
            }
        }

        return filename.substr(start, end - start);
    }
}
