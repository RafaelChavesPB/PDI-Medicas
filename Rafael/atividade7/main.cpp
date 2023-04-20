#include <opencv2/opencv.hpp>
using namespace cv;
int main(){
    Mat original = imread("../resources/ball.jpg", IMREAD_COLOR);
    imwrite("../generated/original.jpg", original);
    return 0;
}