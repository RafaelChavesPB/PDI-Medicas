#include "../../helpers/helpers.h"
#include <stack>
#include <vector>

int di[8] = {0, 0, 1, -1, 1, 1, -1, -1};
int dj[8] = {1, -1, 0, 0, 1, -1, -1, 1};

void dfs1(cv::Mat &a, int i, int j){
    std::stack<std::pair<int, int>> s;
    s.push({i, j});
    while(!s.empty()){
        auto [row, col] = s.top();
        s.pop();
        a.at<uchar>(row, col) = 0;
        for(int k = 0; k < 4; k++){
            int nxi = di[k] + row;
            int nxj = dj[k] + col;
            if(not (nxi >= 0 and nxi < a.rows and nxj >= 0 and nxj < a.cols)) continue;
            uchar point = a.at<uchar>(nxi, nxj);
            if(point > 20) s.push({nxi, nxj});
        }
    }
}

void dfs2(cv::Mat &a, int i, int j, std::vector<std::vector<bool>> &vis){
    std::stack<std::pair<int, int>> s;
    s.push({i, j});
    while(!s.empty()){
        auto [row, col] = s.top();
        s.pop();
        vis[row][col] = true;
        a.at<uchar>(row, col) = 128;
        for(int k = 0; k < 8; k++){
            int nxi = di[k] + row;
            int nxj = dj[k] + col;
            if(not (nxi >= 0 and nxi < a.rows and nxj >= 0 and nxj < a.cols)) continue;
            uchar &point = a.at<uchar>(nxi, nxj);
            if(point == 255) s.push({nxi, nxj});
        }
    }
}

void dfs3(cv::Mat &a, int i, int j){
    std::stack<std::pair<int, int>> s;
    s.push({i, j});
    while(!s.empty()){
        auto [row, col] = s.top();
        s.pop();
        a.at<uchar>(row, col) = 255;
        for(int k = 0; k < 4; k++){
            int nxi = di[k] + row;
            int nxj = dj[k] + col;
            if(not (nxi >= 0 and nxi < a.rows and nxj >= 0 and nxj < a.cols)) continue;
            uchar &point = a.at<uchar>(nxi, nxj);
            if(point < 20) s.push({nxi, nxj});
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

void drawCircle(cv::Mat &src, std::vector<cv::Vec3f> &circles){
    for( size_t idx = 0; idx < circles.size(); idx++)
    {
    cv::Vec3i c = circles[idx];
    cv::Point center = cv::Point(c[0], c[1]);
    // circle outline
    int radius = c[2];
    // cv::putText(src,std::to_string(idx + 1),center, cv::FONT_HERSHEY_PLAIN, 1 ,cv::Scalar(0), 1,cv::LINE_8);
    cv::circle(src, center, radius, cv::Scalar(0), 2, cv::LINE_8);
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
        
        printf("\nStarted rocessing \"%s\" image...\n\n", imgName.c_str());

        // Creates folder
        std::string folderName = "Results-IMG[" + imgName + "]";
        createFolder(folderName.c_str());

        // Sets path variabel for future work
        std::string path = "";
        
        createFolder((folderName + "/1-default").c_str());
        createFolder((folderName + "/2-first_part").c_str());
        createFolder((folderName + "/3-second_part").c_str());
        createFolder((folderName + "/4-final_part").c_str());
        createFolder((folderName + "/4-final_part/all_color").c_str());
        createFolder((folderName + "/3-second_part/first_kill").c_str());
        createFolder((folderName + "/3-second_part/second_kill").c_str());
        path = folderName + "/1-default/";
        /////////////////////////// Gray Images //////////////////////////////////

        // Gray Image
        cv::Mat imgGray = cv::imread(argv[i + 1], cv::IMREAD_GRAYSCALE);

        // Escrevendo as imagens
        cv::imwrite(path + "original.png", image);
        cv::imwrite(path + "grayscale.png", imgGray);
        
        int num = 0;
        /////////////////////////////////////////// PARTE 1 ///////////////////////////////////////////////////////////
        /* 
            Detect circles in image, fill inside it with white color. Otherwise change pixel value to black.
            It should give the components from all balls except the football.
        */ 
        std::cerr << "\nFirst part...\n";
    
        path = folderName + "/2-first_part/";
        
        std::vector<cv::Vec3f> circles;
        cv::Mat parte1 = imgGray.clone();
        cv::HoughCircles(parte1, circles, cv::HOUGH_GRADIENT, 1,
                    imgGray.rows/64,  // change this value to detect circles with different distances to each other
                    100, 40, 25, 45 // change the last two parameters
        );
        std::cerr << "Found " << circles.size() << " circles\n";
        drawCircle(parte1, circles);
        cv::imwrite(path + std::to_string(++num) + "circle_drawn.png", parte1);
        dfs1(parte1, 0, 0);
        cv::imwrite(path + std::to_string(++num) + "after_dfs.png", parte1);

        for(int r = 0; r < parte1.rows; r++){
            for(int c = 0; c < parte1.cols; c++){
                uchar &point = parte1.at<uchar>(r, c);
                if(point not_eq 0) point = 255;
            }
        }
        cv::imwrite(path + std::to_string(++num) + "bin_components.png", parte1);

        std::cerr << "First part done\n";
        /////////////////////////////////////////// PARTE 2 ///////////////////////////////////////////////////////////
        /* Erase all circles and binarize lefting image, dilate and erode certain number of times.
        It should give the components from football
        */ 

        std::cerr << "\nSecond part...\n";

        path = folderName + "/3-second_part/";

        circles.clear();
        cv::Mat parte2 = imgGray.clone();
        cv::HoughCircles(parte2, circles, cv::HOUGH_GRADIENT, 1,
                    imgGray.rows/64,  // change this value to detect circles with different distances to each other
                    100, 40, 25, 45 // change the last two parameters
        );
        std::cerr << "Found " << circles.size() << " circles\n";

        drawCircle(parte2, circles);
        cv::imwrite(path + std::to_string(++num) + "circle_drawn.png", parte2);

        path = folderName + "/3-second_part/first_kill/";
        for(int idx = 0; idx < circles.size(); idx++){
            cv::Vec3i c = circles[idx];
            dfs1(parte2, c[1], c[0]);
            cv::imwrite(path + std::to_string(++num) + "dfs" + std::to_string(idx + 1) + ".png", parte2);
        }
        cv::imwrite(path + std::to_string(++num) + "after_all_first_dfs.png", parte2);

        path = folderName + "/3-second_part/second_kill/";
        for(int idx = 0; idx < circles.size(); idx++){
            cv::Vec3i c = circles[idx];
            dfs3(parte2, c[1], c[0]);
            cv::imwrite(path + std::to_string(++num) + "dfs" + std::to_string(idx + 1) + ".png", parte2);
        }
        cv::imwrite(path + std::to_string(++num) + "after_all_second_dfs.png", parte2);

        path = folderName + "/3-second_part/";
        cv::threshold(parte2, parte2, 130, 255, cv::THRESH_BINARY_INV);
        cv::imwrite(path + std::to_string(++num) + "binarized.png", parte2);

        for(int k = 0; k < 4; k++)
            cv::dilate(parte2, parte2, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)));
        for(int k = 0; k < 6; k++)
            cv::erode(parte2, parte2, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)));
        for(int k = 0; k < 3; k++)
            cv::dilate(parte2, parte2, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)));
        cv::dilate(parte2, parte2, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 3)));
        cv::dilate(parte2, parte2, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 5)));
        
        cv::imwrite(path + std::to_string(++num) + "dilate_erode_process.png", parte2);

        std::cerr << "Second part done.\n";
////////////////////////////////////////////////// PROCESSANDO COMPONENTES /////////////////////////////////////
        /* If a wite pixel is found, run a dfs from it, the result should be the component */

        std::cerr << "\nLooking for components...\n";

        std::vector<cv::Mat> ans;
        cv::Mat component;
        std::vector<std::vector<bool>> vis1(parte1.rows, std::vector<bool>(parte1.cols, 0));
        std::vector<std::vector<bool>> vis2(parte2.rows, std::vector<bool>(parte2.cols, 0));

        for(int r = 0; r < parte1.rows; r++){
            for(int c = 0; c < parte1.cols; c++){
                uchar &point = parte1.at<uchar>(r, c);
                if(not vis1[r][c] and point == 255){
                    std::cerr << "Component starting in " << r << ' ' << c << " from part 1";
                    component = parte1.clone();
                    dfs2(component, r, c, vis1);
                    std::cerr << " -> dfs done" << '\n';
                    for(int i = 0; i < component.rows; i++){
                        for(int j = 0; j < component.cols; j++){
                            uchar &p = component.at<uchar>(i, j);
                            p = p == 128? 255 : 0;
                        }
                    }
                    ans.push_back(component);
                }
                uchar &p2 = parte2.at<uchar>(r, c);
                if(not vis2[r][c] and p2 == 255){
                    std::cerr << "Component starting in " << r << ' ' << c << " from part 2";
                    component = parte2.clone();
                    dfs2(component, r, c, vis2);
                    std::cerr << " -> dfs done" << '\n';
                    for(int i = 0; i < component.rows; i++){
                        for(int j = 0; j < component.cols; j++){
                            uchar &p = component.at<uchar>(i, j);
                            p = p == 128? 255 : 0;
                        }
                    }
                    ans.push_back(component);
                }
            }
        }

        /////////////////////////////////////////// Final Result //////////////////////////////////////////////////
        /* Go in each component and get the result */
        std::cerr << "Found " << ans.size() << " components\n\n";
        int x = 1;

        cv::Mat temp;
        for(auto &item : ans){
            createFolder((folderName + "/4-final_part/Ball" + std::to_string(x++)).c_str());
            path = folderName + "/4-final_part/Ball" + std::to_string(x - 1) + "/";
            cv::imwrite(path + std::to_string(++num) + "mask.png", item);
            temp = item & imgGray;
            cv::imwrite(path + std::to_string(++num) + "gray.png", temp);
            multiply_color(image, temp, item);
            cv::imwrite(path + std::to_string(++num) + "color.png", temp);
            path = folderName + "/4-final_part/all_color/";
            cv::imwrite(path + std::to_string(++num) + "ball" + std::to_string(x - 1) + ".png", temp);
        }

        printf("\nProcessing for \"%s\" has ended.\n", imgName.c_str());

    }
    return 0;
}