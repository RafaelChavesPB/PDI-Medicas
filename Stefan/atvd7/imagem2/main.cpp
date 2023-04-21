#include "../../helpers/helpers.h"
#include <stack>
#include <vector>

int di[8] = {0, 0, 1, -1, 1, 1, -1, -1};
int dj[8] = {1, -1, 0, 0, 1, -1, -1, 1};

void dfs(cv::Mat &a, std::vector<std::vector<bool>> &vis, int i, int j, int &points){
    std::stack<std::pair<int, int>> s;
    s.push({i, j});
    while(!s.empty()){
        auto [row, col] = s.top();
        s.pop();
        points++;
        vis[row][col] = true;
        a.at<uchar>(row, col) = 1;
        for(int k = 0; k < 8; k++){
            int nxi = di[k] + row;
            int nxj = dj[k] + col;
            uchar point = a.at<uchar>(nxi, nxj);
            if(point > 20 and !vis[nxi][nxj]) s.push({nxi, nxj});
        }
    }
}


void multiply_color(cv::Mat &src, cv::Mat &dst, cv::Mat &bin){
    dst = src.clone();
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            cv::Vec3b pixel = src.at<cv::Vec3b>(i, j);
            for(int k = 0; k < 3; k++){
                dst.at<cv::Vec3b>(i, j)[k] = (bin.at<uchar>(i, j) == 255) * pixel[k];
            }
        }
    }
}

void remove_white(cv::Mat &src, int white_thr, int black_thr){
    for(int ty = 0; ty < src.rows; ty++){
        for(int tx = 0; tx < src.rows; tx++){
            if(src.at<uchar>(ty, tx) >= white_thr){
                int k1 = ty;
                while(src.at<uchar>(k1, tx) >= white_thr) k1++;
                int k2 = ty;
                while(src.at<uchar>(k2, tx) >= white_thr) k2--;
                if(src.at<uchar>(k1, tx) < black_thr or src.at<uchar>(k2, tx) < black_thr)
                    src.at<uchar>(ty, tx) = 0;
            }
        }
    }
}

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

        // Gray Image
        cv::Mat imgGray = cv::imread(argv[i + 1], cv::IMREAD_GRAYSCALE);
        cv::Mat gray_blurred;
        cv::blur(imgGray, gray_blurred, cv::Size(5,5));

        // Escrevendo as imagens
        path = folderName + "/";
        cv::imwrite(path + "original.png", image);
        cv::imwrite(path + "original_grayscale.png", imgGray);
        cv::imwrite(path + "grayscale_blurred.png", gray_blurred);

        createFolder((folderName + (std::string)"/1-Pre-processamento").c_str());
        path = folderName + "/1-Pre-processamento/";
        int num = 0; // counts number of image
        // Sobel
        cv::Mat sobel_x, sobel_y, sobel_xy;
        cv::Mat sobel_x_kernel = (cv::Mat_<char>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
        cv::Mat sobel_y_kernel = (cv::Mat_<char>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
        cv::filter2D(imgGray, sobel_x, CV_16S, sobel_x_kernel);
        cv::filter2D(imgGray, sobel_y, CV_16S, sobel_y_kernel);
        cv::convertScaleAbs(sobel_x, sobel_x);
        cv::convertScaleAbs(sobel_y, sobel_y);
        cv::addWeighted(sobel_x, 0.5, sobel_y, 0.5, 0, sobel_xy);
        cv::imwrite(path + std::to_string(++num) + "sobel.png", sobel_xy);
        
        // Dilate Sobel
        cv::Mat temp = sobel_xy.clone();
        int op = 15, erode_time = 7, bin_time = 1;
        for(int k = 0; k < op; k++){
            if(k == erode_time){
                cv::erode(temp, temp, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5)));
                cv::imwrite(path + std::to_string(++num) + "eroded" + std::to_string(k + 1) + ".png", temp);
            }else if(k == bin_time){
                for(int y = 0; y < temp.rows; y++){
                    for(int x = 0; x < temp.cols; x++){
                        uchar &point = temp.at<uchar>(y, x);
                        point = point < 20 ? 0 : 255;
                    }
                }
                cv::imwrite(path + std::to_string(++num) + "dilated_bin" + std::to_string(k + 1) + ".png", temp);
            }else{
                cv::dilate(temp, temp, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5)));
                cv::imwrite(path + std::to_string(++num) + "dilated" + std::to_string(k + 1) + ".png", temp);
            }
        }

        ///////////////////////////////////////////////////
        
        createFolder((folderName + (std::string)"/2-Maybe").c_str());
        path = folderName + "/2-Maybe/";

        cv::Mat maybe = temp & imgGray;
        cv::imwrite(path + std::to_string(++num) + "bitwise_with_gray.png", maybe);
        
        // Remove White color
        for(int l = 0; l < maybe.rows; l++){
            for(int c = 0; c < maybe.cols; c++){
                uchar &point = maybe.at<uchar>(l, c);
                if(point >= 250) point = 0;
            }
        }
        cv::imwrite(path + std::to_string(++num) + "removed_white.png", maybe);

        // Blur
        for(int k = 0; k < 2; k++) blur(maybe, maybe, cv::Size(5, 5));
        cv::imwrite(path + std::to_string(++num) + "blurred.png", maybe);
        
        for(int l = 0; l < maybe.rows; l++){
            for(int c = 0; c < maybe.cols; c++){
                uchar &point = maybe.at<uchar>(l, c);
                if(point > 20) point = 255;
                else point = 0;
            }
        }
        cv::imwrite(path + std::to_string(++num) + "binarized.png", maybe);

        op = 4;
        for(int k = 0; k < op; k++){
            if(k >= 2){
                cv::erode(maybe, maybe, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5)));
                cv::imwrite(path + std::to_string(++num) + "eroded_bin" + std::to_string(k + 1) + ".png", maybe);
            }else{
                cv::dilate(maybe, maybe, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5)));
                cv::imwrite(path + std::to_string(++num) + "dilated_bin" + std::to_string(k + 1) + ".png", maybe);
            }
        }

        // Visited vector for dfs
        std::vector<std::vector<bool>> visited(maybe.rows, std::vector<bool>(maybe.cols, 0));

        createFolder((folderName + (std::string)"/3-Find").c_str());
        createFolder((folderName + (std::string)"/4-Gray").c_str());
        createFolder((folderName + (std::string)"/5-Color").c_str());
        path = folderName + "/3-Find/";

        int cont = 0, b = 0;
        for(int y = 0; y < maybe.rows; y++){
            for(int x = 0; x < maybe.cols; x++){
                uchar point = maybe.at<uchar>(y, x);
                if(point < 20 or visited[y][x]){
                    continue;
                }
                cont++;
                cv::Mat ball = maybe.clone();
                int points = 0;
                dfs(ball, visited, y, x, points);
                if(points < 1000) continue;
                b++;
                std::cerr << b << ". dfs called from " << y << ' ' << x << " was successfull" << '\n';

                for(int row = 0; row < ball.rows; row++){
                    for(int col = 0; col < ball.cols; col++){
                        auto &p = ball.at<uchar>(row, col);
                        p = p == 1? 255 : 0;
                    }
                }
                path = folderName + "/3-Find/";

                cv::imwrite(path + std::to_string(++num) + "after_dfs" + std::to_string(b) + ".png", ball);

                for(int k = 0; k < 3; k++){
                    cv::erode(ball, ball, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)));
                }
                cv::imwrite(path + std::to_string(++num) + "erode_after_dfs" + std::to_string(b) + ".png", ball);

                // Do detailed processing for images 7 and 9
                switch(b){
                    case 7:
                        for(int k = 0; k < 4; k++){
                            cv::dilate(ball, ball, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
                            cv::dilate(ball, ball, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5)));
                        }
                        for(int k = 0; k < 10; k++)
                            cv::erode(ball, ball, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)));
                        break;
                    
                    case 9:
                        for(int k = 0; k < 7; k++)
                            cv::dilate(ball, ball, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));
                        
                        for(int k = 0; k < 2; k++)
                            blur(ball, ball, cv::Size(3, 3));
                        
                        for(int k = 0; k < 7; k++)
                            cv::erode(ball, ball, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)));
                        break;

                    default:
                        break;
                }

                // Create Mask
                for(int row = 0; row < ball.rows; row++){
                    for(int col = 0; col < ball.cols; col++){
                        auto &p = ball.at<uchar>(row, col);
                        p = p not_eq 0? 255 : 0;
                    }
                }

                cv::imwrite(path + std::to_string(++num) + "mask_ball" + std::to_string(b) + ".png", ball);

                cv::Mat final_result;
                multiply_color(image, final_result, ball);
                cv::Mat gray_result = ball & imgGray;

                path = folderName + "/4-Gray/";
                cv::imwrite(path + std::to_string(++num) + "ball" + std::to_string(b) + ".png", gray_result);
                path = folderName + "/5-Color/";
                cv::imwrite(path + std::to_string(++num) + "color_ball" + std::to_string(b) + ".png", final_result);

            }
        }
        std::cerr << "Code called dfs " << cont << " times but only " << b << " were successfull" << '\n';

    }
    return 0;
}