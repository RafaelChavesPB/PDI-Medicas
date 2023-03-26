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
        cv::Mat imgGray = cv::imread(argv[i + 1], cv::IMREAD_GRAYSCALE);
        
        printf("Spatial Info for %s = %.2f\n", imgName.c_str(), calculateSpatialInfo(imgGray));

        cv::Mat sobel_x, sobel_y, sobel_xy;
        cv::Mat sobel_x_kernel = (cv::Mat_<char>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
        cv::Mat sobel_y_kernel = (cv::Mat_<char>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
        cv::filter2D(imgGray, sobel_x, CV_16S, sobel_x_kernel);
        cv::filter2D(imgGray, sobel_y, CV_16S, sobel_y_kernel);
        cv::convertScaleAbs(sobel_x, sobel_x);
        cv::convertScaleAbs(sobel_y, sobel_y);
        cv::addWeighted(sobel_x, 0.5, sobel_y, 0.5, 0, sobel_xy);
        printf("Sobel done\n");

        cv::Mat roberts_x, roberts_y, roberts_xy;
        cv::Mat roberts_x_kernel = (cv::Mat_<char>(2, 2) << 1, 0, 0, -1);
        cv::Mat roberts_y_kernel = (cv::Mat_<char>(2, 2) << 0, 1, -1, 0);
        cv::filter2D(imgGray, roberts_x, CV_16S, roberts_x_kernel);
        cv::filter2D(imgGray, roberts_y, CV_16S, roberts_y_kernel);
        cv::convertScaleAbs(roberts_x, roberts_x);
        cv::convertScaleAbs(roberts_y, roberts_y);
        cv::addWeighted(roberts_x, 0.5, roberts_y, 0.5, 0, roberts_xy);
        printf("Roberts done\n");

        cv::Mat prewitt_x, prewitt_y, prewitt_xy;
        cv::Mat prewitt_x_kernel = (cv::Mat_<char>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
        cv::Mat prewitt_y_kernel = (cv::Mat_<char>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
        cv::filter2D(imgGray, prewitt_x, CV_16S, prewitt_x_kernel);
        cv::filter2D(imgGray, prewitt_y, CV_16S, prewitt_y_kernel);
        cv::convertScaleAbs(prewitt_x, prewitt_x);
        cv::convertScaleAbs(prewitt_y, prewitt_y);
        cv::addWeighted(prewitt_x, 0.5, prewitt_y, 0.5, 0, prewitt_xy);
        printf("Prewitt done\n");

        // O operador Laplaciano é um filtro isotrópico que detecta bordas independentemente da orientação
        cv::Mat laplacian_xy, laplacian_abs;
        cv::Laplacian(imgGray, laplacian_xy, CV_16S, 1);
        cv::convertScaleAbs(laplacian_xy, laplacian_abs);
        printf("Laplacian done\n");

        cv::Mat canny;
        cv::Canny(imgGray, canny, 100, 200);
        printf("Canny done\n");


        // Gerando os resultados
        createFolder((folderName + "/BORDAS").c_str());
        path = folderName + "/";
        cv::imwrite(path + "original.png", image);
        cv::imwrite(path + "original_grayscale.png", imgGray);
        
        // Gerando os resultados para deteccao de bordas
        // Sobel
        createFolder((folderName + "/BORDAS/Sobel").c_str());
        path = folderName + "/BORDAS/Sobel/";
        cv::imwrite(path + "x.png", sobel_x);
        cv::imwrite(path + "y.png", sobel_y);
        cv::imwrite(path + "xy.png", sobel_xy);

        // Roberts
        createFolder((folderName + "/BORDAS/Roberts").c_str());
        path = folderName + "/BORDAS/Roberts/";
        cv::imwrite(path + "x.png", roberts_x);
        cv::imwrite(path + "y.png", roberts_y);
        cv::imwrite(path + "xy.png", roberts_xy);

        // Prewitt
        createFolder((folderName + "/BORDAS/Prewitt").c_str());
        path = folderName + "/BORDAS/Prewitt/";
        cv::imwrite(path + "x.png", prewitt_x);
        cv::imwrite(path + "y.png", prewitt_y);
        cv::imwrite(path + "xy.png", prewitt_xy);

        // Laplacian
        createFolder((folderName + "/BORDAS/Laplacian").c_str());
        path = folderName + "/BORDAS/Laplacian/";
        cv::imwrite(path + "xy.png", laplacian_abs);

        // Canny
        createFolder((folderName + "/BORDAS/Canny").c_str());
        path = folderName + "/BORDAS/Canny/";
        cv::imwrite(path + "canny.png", canny);

        printf("Processed image %s\n", imgName.c_str());

    }
    return 0;
}