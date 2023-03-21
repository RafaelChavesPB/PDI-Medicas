#include "../helpers/helpers.h"

int main(int argc, char** argv){
    if(argc < 2){
        printf("This script needs at least one argument, which must be a image file\n");
        return 0;
    }
    for(int i = 0; i < argc - 1; i++){
        // Reads Image
        cv::Mat image = cv::imread(argv[i + 1]);

        // Gets filename
        std::string imgName = argv[i + 1];
        clearString(imgName);
        
        printf("\nProcessing \"%s\" image...\n", imgName.c_str());

        // Creates folder
        std::string folderName = "Results-IMG[" + imgName + "]";
        createFolder(folderName.c_str());

        // Sets path variabel for future work
        std::string path = "";

        /////////////////////////// Gray Images //////////////////////////////////

        // Gray Image by mean and by weight
        cv::Mat imgGrayMean = cv::Mat::zeros(image.size(), CV_8UC1);
        cv::Mat imgGrayWeight = cv::Mat::zeros(image.size(), CV_8UC1);

        for(int i = 0; i < image.rows; i++) {
            for(int j = 0; j < image.cols; j++) {
                cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
                imgGrayMean.at<uchar>(i, j) = getGrayValueFromBGRPixel(pixel, true);
                imgGrayWeight.at<uchar>(i, j) = getGrayValueFromBGRPixel(pixel, false);
            }
        }
        
        printf("Spatial Info for %s = %.2f\n", imgName.c_str(), calculateSpatialInfo(imgGrayWeight));

         // Folder for writing gray image results
        path = folderName + "/GRAY";
        createFolder(path.c_str());
        
        cv::imwrite(path + "/mean.png", imgGrayMean);
        cv::imwrite(path + "/weight.png", imgGrayWeight);
        
        ////////////////////////////// HSV Images //////////////////////////////////
        // Converting image
        cv::Mat hsv_img, hsv_channel[3];
        cv::cvtColor(image, hsv_img, cv::COLOR_BGR2HSV);
        cv::split(hsv_img, hsv_channel);

        
        // int maxval = -1;
        // for(int i = 0; i < hsv_img.rows; i++){
        //     for(int j = 0; j < hsv_img.cols; j++){
        //         cv::Vec3b pixel = hsv_img.at<cv::Vec3b>(i, j);
        //         printf("[%d, %d, %d] ", pixel[0], pixel[1], pixel[2]);
        //         if(pixel[0] > maxval) maxval = pixel[0];
        //     }
        //     printf("\n");
        // }
        // printf("Max val = %d\n", maxval);

        // Folder for hsv images
        path = folderName + "/HSV";
        createFolder(path.c_str());

        cv::imwrite(path + "/ORIG_hsv.png", hsv_img);
        cv::imwrite(path + "/HUE_hsv.png", hsv_channel[0]);
        cv::imwrite(path + "/SATURATION_hsv.png", hsv_channel[1]);
        cv::imwrite(path + "/VALUE_hsv.png", hsv_channel[2]);


        ///////////////////////////// Color Images //////////////////////////////////
        // Folder for color processing
        path = folderName + "/COLOR";
        createFolder(path.c_str());
        std::string cp_path = "";
        int step = 15;
        int maxlim = step * 12 + 1;

        for(int lim = step, idx = 0; lim < maxlim; lim += step, idx++){
            cv::Mat mask, result;
            cv::Scalar lower = cv::Scalar(lim - step, 0, 0);
            cv::Scalar upper = cv::Scalar(lim - 1, 255, 255);

            cv::inRange(hsv_img, lower, upper, mask);
            cv::bitwise_and(image, image, result, mask);

            cp_path = path + "/" + colorNames[idx];
            createFolder(cp_path.c_str());
            cv::imwrite(cp_path + "/mask.png", mask);
            cv::imwrite(cp_path + "/result.png", result);
        }

    }
    return 0;
}