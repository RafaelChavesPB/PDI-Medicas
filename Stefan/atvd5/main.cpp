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
        cv::Mat blurred_imgGray;
        double pct = .4 / 100; // takes 0.4% of the max size of image to determine size of filtering box
        cv::Size imageSize = imgGray.size();
        int val = std::max(imageSize.height, imageSize.width) * pct;

        cv::blur(imgGray, blurred_imgGray, cv::Size(val, val));
        
        printf("Spatial Info for %s = %.2f\n", imgName.c_str(), calculateSpatialInfo(imgGray));

        cv::Mat sobel_x, sobel_y, sobel_xy;
        cv::Mat blurred_sobel_x, blurred_sobel_y, blurred_sobel_xy;
        cv::Mat sobel_x_kernel = (cv::Mat_<char>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
        cv::Mat sobel_y_kernel = (cv::Mat_<char>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
        cv::filter2D(imgGray, sobel_x, CV_16S, sobel_x_kernel);
        cv::filter2D(imgGray, sobel_y, CV_16S, sobel_y_kernel);
        cv::filter2D(imgGray, blurred_sobel_x, CV_16S, sobel_x_kernel);
        cv::filter2D(imgGray, blurred_sobel_y, CV_16S, sobel_y_kernel);
        cv::convertScaleAbs(sobel_x, sobel_x);
        cv::convertScaleAbs(sobel_y, sobel_y);
        cv::convertScaleAbs(blurred_sobel_x, blurred_sobel_x);
        cv::convertScaleAbs(blurred_sobel_y, blurred_sobel_y);
        cv::addWeighted(sobel_x, 0.5, sobel_y, 0.5, 0, sobel_xy);
        cv::addWeighted(blurred_sobel_x, 0.5, blurred_sobel_y, 0.5, 0, blurred_sobel_xy);
        printf("Sobel done\n");

        cv::Mat roberts_x, roberts_y, roberts_xy;
        cv::Mat blurred_roberts_x, blurred_roberts_y, blurred_roberts_xy;
        cv::Mat roberts_x_kernel = (cv::Mat_<char>(2, 2) << 1, 0, 0, -1);
        cv::Mat roberts_y_kernel = (cv::Mat_<char>(2, 2) << 0, 1, -1, 0);
        cv::filter2D(imgGray, roberts_x, CV_16S, roberts_x_kernel);
        cv::filter2D(imgGray, roberts_y, CV_16S, roberts_y_kernel);
        cv::filter2D(imgGray, blurred_roberts_x, CV_16S, roberts_x_kernel);
        cv::filter2D(imgGray, blurred_roberts_y, CV_16S, roberts_y_kernel);
        cv::convertScaleAbs(roberts_x, roberts_x);
        cv::convertScaleAbs(roberts_y, roberts_y);
        cv::convertScaleAbs(blurred_roberts_x, blurred_roberts_x);
        cv::convertScaleAbs(blurred_roberts_y, blurred_roberts_y);
        cv::addWeighted(roberts_x, 0.5, roberts_y, 0.5, 0, roberts_xy);
        cv::addWeighted(blurred_roberts_x, 0.5, blurred_roberts_y, 0.5, 0, blurred_roberts_xy);
        printf("Roberts done\n");

        cv::Mat prewitt_x, prewitt_y, prewitt_xy;
        cv::Mat blurred_prewitt_x, blurred_prewitt_y, blurred_prewitt_xy;
        cv::Mat prewitt_x_kernel = (cv::Mat_<char>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
        cv::Mat prewitt_y_kernel = (cv::Mat_<char>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
        cv::filter2D(imgGray, prewitt_x, CV_16S, prewitt_x_kernel);
        cv::filter2D(imgGray, prewitt_y, CV_16S, prewitt_y_kernel);
        cv::filter2D(imgGray, blurred_prewitt_x, CV_16S, prewitt_x_kernel);
        cv::filter2D(imgGray, blurred_prewitt_y, CV_16S, prewitt_y_kernel);
        cv::convertScaleAbs(prewitt_x, prewitt_x);
        cv::convertScaleAbs(prewitt_y, prewitt_y);
        cv::convertScaleAbs(blurred_prewitt_x, blurred_prewitt_x);
        cv::convertScaleAbs(blurred_prewitt_y, blurred_prewitt_y);
        cv::addWeighted(prewitt_x, 0.5, prewitt_y, 0.5, 0, prewitt_xy);
        cv::addWeighted(blurred_prewitt_x, 0.5, blurred_prewitt_y, 0.5, 0, blurred_prewitt_xy);
        printf("Prewitt done\n");

        // filtro gaussiano for LoG
        cv::Mat gaussian, blurred_gaussian;
        cv::GaussianBlur(imgGray, gaussian, cv::Size(5, 5), 0);
        cv::GaussianBlur(imgGray, blurred_gaussian, cv::Size(5, 5), 0);

        // O operador Laplaciano é um filtro isotrópico que detecta bordas independentemente da orientação
        cv::Mat laplacian_xy, laplacian_abs, gaussian_laplacian_xy, blurred_gaussian_laplacian_xy;
        cv::Mat blurred_laplacian_xy, blurred_laplacian_abs;
        cv::Laplacian(imgGray, laplacian_xy, CV_16S, 1);
        cv::Laplacian(blurred_imgGray, blurred_laplacian_xy, CV_16S, 1);
        cv::Laplacian(gaussian, gaussian_laplacian_xy, CV_16S, 1);
        cv::Laplacian(blurred_gaussian, blurred_gaussian_laplacian_xy, CV_16S, 1);

        cv::convertScaleAbs(laplacian_xy, laplacian_abs);
        cv::convertScaleAbs(blurred_laplacian_xy, blurred_laplacian_abs);
        cv::convertScaleAbs(gaussian_laplacian_xy, gaussian_laplacian_xy);
        cv::convertScaleAbs(blurred_gaussian_laplacian_xy, blurred_gaussian_laplacian_xy);

        printf("Laplacian done\n");

        cv::Mat canny, blurred_canny;
        cv::Canny(imgGray, canny, 100, 200);
        cv::Canny(blurred_imgGray, blurred_canny, 100, 200);
        printf("Canny done\n");

        // converter a imagem para um vetor de pontos
        cv::Mat points;
        image.reshape(1, image.rows * image.cols).convertTo(points, CV_32F);

        // executar o algoritmo k-means
        int K = 4; // número de clusters
        cv::Mat labels, centers;
        kmeans(points, K, labels, cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::MAX_ITER, 10, 1.0), 3, cv::KMEANS_PP_CENTERS, centers);

        // atualizar as cores dos pixels na imagem original
        cv::Mat kmeans_image(image.size(), image.type());
        for (int i = 0; i < points.rows; ++i)
        {
            int cluster_id = labels.at<int>(i);
            kmeans_image.at<cv::Vec3b>(i) = centers.at<cv::Vec3f>(cluster_id);
        }
        printf("Kmeans done\n");


        // Gerando os resultados
        createFolder((folderName + "/BORDAS").c_str());
        path = folderName + "/";
        cv::imwrite(path + "original.png", image);
        cv::imwrite(path + "original_grayscale.png", imgGray);
        cv::imwrite(path + "original_blurred_grayscale.png", blurred_imgGray);

        
        // Gerando os resultados para deteccao de bordas
        // Sobel
        createFolder((folderName + "/BORDAS/Sobel").c_str());
        createFolder((folderName + "/BORDAS/Sobel/Unfiltered").c_str());
        createFolder((folderName + "/BORDAS/Sobel/Filtered").c_str());

        path = folderName + "/BORDAS/Sobel/Unfiltered/";
        cv::imwrite(path + "x.png", sobel_x);
        cv::imwrite(path + "y.png", sobel_y);
        cv::imwrite(path + "xy.png", sobel_xy);
        path = folderName + "/BORDAS/Sobel/Filtered/";
        cv::imwrite(path + "x.png", blurred_sobel_x);
        cv::imwrite(path + "y.png", blurred_sobel_y);
        cv::imwrite(path + "xy.png", blurred_sobel_xy);


        // Roberts
        createFolder((folderName + "/BORDAS/Roberts").c_str());
        createFolder((folderName + "/BORDAS/Roberts/Unfiltered").c_str());
        createFolder((folderName + "/BORDAS/Roberts/Filtered").c_str());

        path = folderName + "/BORDAS/Roberts/Unfiltered/";
        cv::imwrite(path + "x.png", roberts_x);
        cv::imwrite(path + "y.png", roberts_y);
        cv::imwrite(path + "xy.png", roberts_xy);
        path = folderName + "/BORDAS/Roberts/Filtered/";
        cv::imwrite(path + "x.png", blurred_roberts_x);
        cv::imwrite(path + "y.png", blurred_roberts_y);
        cv::imwrite(path + "xy.png", blurred_roberts_xy);

        // Prewitt
        createFolder((folderName + "/BORDAS/Prewitt").c_str());
        createFolder((folderName + "/BORDAS/Prewitt/Unfiltered").c_str());
        createFolder((folderName + "/BORDAS/Prewitt/Filtered").c_str());

        path = folderName + "/BORDAS/Prewitt/Unfiltered/";
        cv::imwrite(path + "x.png", prewitt_x);
        cv::imwrite(path + "y.png", prewitt_y);
        cv::imwrite(path + "xy.png", prewitt_xy);
        path = folderName + "/BORDAS/Prewitt/Filtered/";
        cv::imwrite(path + "x.png", blurred_prewitt_x);
        cv::imwrite(path + "y.png", blurred_prewitt_y);
        cv::imwrite(path + "xy.png", blurred_prewitt_xy);

        // Laplacian
        createFolder((folderName + "/BORDAS/Laplacian").c_str());
        createFolder((folderName + "/BORDAS/Laplacian/Unfiltered").c_str());
        createFolder((folderName + "/BORDAS/Laplacian/Filtered").c_str());

        path = folderName + "/BORDAS/Laplacian/Unfiltered/";
        cv::imwrite(path + "xy.png", laplacian_abs);
        cv::imwrite(path + "gaussian_xy.png", gaussian_laplacian_xy);
        path = folderName + "/BORDAS/Laplacian/Filtered/";
        cv::imwrite(path + "xy.png", blurred_laplacian_abs);
        cv::imwrite(path + "gaussian_xy.png", blurred_gaussian_laplacian_xy);

        // Canny
        createFolder((folderName + "/BORDAS/Canny").c_str());
        createFolder((folderName + "/BORDAS/Canny/Unfiltered").c_str());
        createFolder((folderName + "/BORDAS/Canny/Filtered").c_str());
        path = folderName + "/BORDAS/Canny/Unfiltered/";
        cv::imwrite(path + "canny.png", canny);
        path = folderName + "/BORDAS/Canny/Filtered/";
        cv::imwrite(path + "canny.png", blurred_canny);

        // Kmeans

        createFolder((folderName + "/KMEANS").c_str());
        path = folderName + "/KMEANS/";
        cv::imwrite(path + "kmeans.png", kmeans_image);

        printf("Processed image %s\n", imgName.c_str());

    }
    return 0;
}