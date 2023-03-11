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

        // Creates folder
        std::string folderName = "ResultsIMG[" + imgName + "]";
        createFolder(folderName.c_str());

        // Folder for gray images
        std::string path = folderName + "/GRAY";
        createFolder(path.c_str());

        for(int i = 0; i < image.rows; i++) {
            for(int j = 0; j < image.cols; j++) {
                cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
                // Faça algo com o pixel
            }
        }
        cv::imwrite("hohoho.png", image);
    }
    return 0;
}