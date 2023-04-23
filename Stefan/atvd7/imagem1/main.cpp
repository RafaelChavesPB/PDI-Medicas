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
    circle(src, center, radius, cv::Scalar(0), 3, cv::LINE_8);
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
        createFolder((folderName + "/default").c_str());
        createFolder((folderName + "/1-start").c_str());
        createFolder((folderName + "/2-end").c_str());
        createFolder((folderName + "/2-end/all_color").c_str());

        // Sets path variabel for future work
        std::string path = folderName + "/default/";

///////////////////////////////////////// Gray Images //////////////////////////////////

        // Gray Image
        cv::Mat imgGray = cv::imread(argv[i + 1], cv::IMREAD_GRAYSCALE);

        // Write image
        cv::imwrite(path + "original.png", image);
        cv::imwrite(path + "grayscale.png", imgGray);
        
        int num = 0; // counts number of image
        path = folderName + "/1-start/";
/////////////////////////////////////////////////////////////Start /////////////////////////////////////q
    /*
        Find the 9 circular balls, fill them with black color. The other balls are different from background which is white.
        Therefore put them all black.
        In the "peteca" will be observed some gaps, but dont worry just do closing operation followed by blur a few times.
        It should be sufficient to change the gap color to something above 200 pixel value.
        Then just binarize the image... above value 200 go to 255, under go to 0.
    */
        auto img = imgGray.clone();
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(img, circles, cv::HOUGH_GRADIENT, 1,
                    img.rows/16,  // change this value to detect circles with different distances to each other
                    100, 35, 50, 53 // change the last two parameters
        );
        std::cerr << "Found " << circles.size() << " circles\n";
        drawCircle(img, circles);
        cv::imwrite(path + std::to_string(++num) + "circle_drawn.png", img);
        for(auto idx = 0; idx < circles.size(); idx++){
            cv::Vec3i c = circles[idx];
            dfs1(img, c[1], c[0]);
        }
        cv::imwrite(path + std::to_string(++num) + "after_dfs.png", img);

        for(int r = 0; r < img.rows; r++){
            for(int c = 0; c < img.cols; c++){
                uchar &point = img.at<uchar>(r, c);
                if(point <= 250) point = 0;
            }
        }
        cv::imwrite(path + std::to_string(++num) + "after_process.png", img);
        img = ~img;
        cv::imwrite(path + std::to_string(++num) + "inverse.png", img);
        cv::blur(img, img, cv::Size(5, 5));
        cv::imwrite(path + std::to_string(++num) + "after_blur.png", img);

        for(int k = 0; k < 15; k++){
            cv::morphologyEx(img, img, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(9, 9)));
            cv::blur(img, img, cv::Size(3, 3));
        }
        cv::imwrite(path + std::to_string(++num) + "after_close_blur.png", img);

        for(int r = 0; r < img.rows; r++){
            for(int c = 0; c < img.cols; c++){
                uchar &point = img.at<uchar>(r, c);
                if(point >= 200) point = 255;
                else point = 0;
            }
        }
        cv::imwrite(path + std::to_string(++num) + "bin_components.png", img);

        //////////////////////////////// Final part ///////////////////////////////////
        /* If a white pixel is found, run a dfs from it, the result should be the component */

        std::cerr << "\nLooking for components..." << '\n';
        std::vector<cv::Mat> ans;
        cv::Mat component;
        std::vector<std::vector<bool>> vis(img.rows, std::vector<bool>(img.cols, 0));

        for(int r = 0; r < img.rows; r++){
            for(int c = 0; c < img.cols; c++){
                uchar &point = img.at<uchar>(r, c);
                if(not vis[r][c] and point == 255){
                    std::cerr << "Component starting in " << r << ' ' << c;
                    component = img.clone();
                    dfs2(component, r, c, vis);
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
            createFolder((folderName + "/2-end/Ball" + std::to_string(x++)).c_str());
            path = folderName + "/2-end/Ball" + std::to_string(x - 1) + "/";
            cv::imwrite(path + std::to_string(++num) + "mask.png", item);
            temp = item & imgGray;
            cv::imwrite(path + std::to_string(++num) + "gray.png", temp);
            multiply_color(image, temp, item);
            cv::imwrite(path + std::to_string(++num) + "color.png", temp);
            path = folderName + "/2-end/all_color/";
            cv::imwrite(path + std::to_string(++num) + "ball" + std::to_string(x - 1) + ".png", temp);
        }

        printf("\nProcessing for \"%s\" has ended.\n", imgName.c_str());
    }
    return 0;
}