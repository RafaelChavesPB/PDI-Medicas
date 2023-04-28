#include "../../helpers/helpers.h"
#include "color_ranges.h"
#include <stack>
#include <vector>
#include <tuple>

int di[8] = {0, 0, 1, -1, 1, 1, -1, -1};
int dj[8] = {1, -1, 0, 0, 1, -1, -1, 1};

int whichtype(std::map<int, int> &type, cv::Mat &image_hsv, int row, int col);

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

void dfs2(cv::Mat &a, int i, int j, std::vector<std::vector<bool>> &vis, std::map<int, int> &type, cv::Mat &image){
    std::stack<std::pair<int, int>> s;
    s.push({i, j});
    while(!s.empty()){
        auto [row, col] = s.top();
        s.pop();
        vis[row][col] = true;
        a.at<uchar>(row, col) = 128;
        int ball_type = whichtype(type, image, row, col);
        if(ball_type != -1) type[ball_type]++;
        for(int k = 0; k < 8; k++){
            int nxi = di[k] + row;
            int nxj = dj[k] + col;
            if(not (nxi >= 0 and nxi < a.rows and nxj >= 0 and nxj < a.cols) or vis[nxi][nxj]) continue;
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

void drawEllipses(cv::Mat &src, cv::Mat &dst, std::vector<cv::Point2i> &centers){
        std::vector<std::vector<cv::Point> > contours;

        cv::findContours(src, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));
        std::cerr << "contours size = " << contours.size() << '\n';

        cv::RotatedRect rotRecs[contours.size()];
        cv::Mat drawing = cv::Mat::zeros(src.size(), CV_8UC1);
        for(int i = 0; i < contours.size(); i++){
            rotRecs[i] = fitEllipse(contours[i]);
            centers.push_back((cv::Point2i)rotRecs[i].center);
            cv::ellipse(drawing, rotRecs[i], cv::Scalar(255), 1, cv::LINE_8);
        } 
        dst = drawing;
}

bool isInRange(std::tuple<int, int, int> &a, std::tuple<int, int, int> &b, cv::Vec<uchar, 3> &pixel){
    return 
        pixel[0] >= std::get<0>(a) and pixel[0] <= std::get<0>(b) and
        pixel[1] >= std::get<1>(a) and pixel[1] <= std::get<1>(b) and
        pixel[2] >= std::get<2>(a) and pixel[2] <= std::get<2>(b);
}

int whichtype(std::map<int, int> &type, cv::Mat &image_hsv, int row, int col){
    int idx = 0;
    for(auto &x : balls_color_ranges){
        auto t1 = x.first;
        auto t2 = x.second;
        auto pixel = image_hsv.at<cv::Vec3b>(row, col);
        if(isInRange(t1, t2, pixel)) return idx;
        idx++;
    }
    return -1;
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
        createFolder((folderName + "/4-third_part").c_str());
        createFolder((folderName + "/5-final_part").c_str());
        createFolder((folderName + "/5-final_part/all_color").c_str());
        createFolder((folderName + "/3-second_part/first_kill").c_str());
        createFolder((folderName + "/3-second_part/second_kill").c_str());
        path = folderName + "/1-default/";

        // Gray Image
        cv::Mat imgGray = cv::imread(argv[i + 1], cv::IMREAD_GRAYSCALE);

        // Escrevendo as imagens
        cv::imwrite(path + "original.png", image);
        cv::imwrite(path + "grayscale.png", imgGray);
        
        int num = 0;
        
        /////////////////////////////////////////// FIRST PART ///////////////////////////////////////////////////////////
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
        cv::Mat circles_drawn = parte1.clone();
        dfs1(parte1, 0, 0); // all outside circles go to black
        cv::imwrite(path + std::to_string(++num) + "after_dfs.png", parte1);

        for(int r = 0; r < parte1.rows; r++){
            for(int c = 0; c < parte1.cols; c++){
                uchar &point = parte1.at<uchar>(r, c);
                if(point not_eq 0) point = 255;
            }
        }
        cv::imwrite(path + std::to_string(++num) + "bin_components.png", parte1);

        std::cerr << "First part done\n";
        /////////////////////////////////////////// SECOND PART ///////////////////////////////////////////////////////////
        /*  
            Erase all balls identified with circles and binarize lefting football ball. Aplly closing operation certain number of times.
            Run convex hull and get the football component
        */ 

        std::cerr << "\nSecond part...\n";

        path = folderName + "/3-second_part/";

        cv::Mat parte2 = circles_drawn;
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
        cv::Mat hsv_p2;
        cv::cvtColor(parte2, hsv_p2, cv::COLOR_GRAY2BGR);
        cv::cvtColor(hsv_p2, hsv_p2, cv::COLOR_BGR2HSV);

        path = folderName + "/3-second_part/";
    
        cv::threshold(parte2, parte2, 130, 255, cv::THRESH_BINARY_INV);
        cv::imwrite(path + std::to_string(++num) + "binarized.png", parte2);

        bool morph_rect = true;
        int x = 1;
        for(int k = 1; k <= 200; k++){
            if(k % 20 == 0){
                morph_rect = not morph_rect;
                cv::imwrite(path + std::to_string(++num) + "morph" + std::to_string(x++) + ".png", parte2);
            }
            if(morph_rect)
                cv::morphologyEx(parte2, parte2, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5)));
            else
                cv::morphologyEx(parte2, parte2, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5,5)));
        }
        cv::erode(parte2, parte2, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3)));
        cv::imwrite(path + std::to_string(++num) + "erode.png", parte2);

        std::vector< std::vector<cv::Point> > contours; // list of contour points
        std::vector<cv::Vec4i> hierarchy; // necessary for function

        // find contours
        cv::findContours(parte2, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

        // create hull array for convex hull points
        std::vector< std::vector<cv::Point> > hull(contours.size());
        for(int j = 0; j < contours.size(); j++)
            cv::convexHull(cv::Mat(contours[j]), hull[j]);
        
        // create a blank image (black image)
        cv::Mat drawing = cv::Mat::zeros(parte2.size(), CV_8UC1); 
 
        for(int idx = 0; idx < contours.size(); idx++) {
            cv::Scalar color = cv::Scalar(255); // white
            cv::drawContours(drawing, hull, idx, color, 1, cv::LINE_8, std::vector<cv::Vec4i>(), 0, cv::Point());
        }

        cv::imwrite(path + std::to_string(++num) + "hull.png", drawing);
        
        // compute center of each hull element
        std::vector<std::pair<int, int>> centers;
        for(int idx = 0; idx < hull.size(); idx++){
            int x_center = 0, y_center = 0;
            for(int j = 0; j < hull[idx].size(); j++){
                x_center += hull[idx][j].x;
                y_center += hull[idx][j].y;
            }
            x_center /= hull[idx].size();
            y_center /= hull[idx].size();
            centers.push_back({y_center, x_center});
        }

        // fill all components with white color
        for(auto &center_pair : centers) dfs3(drawing, center_pair.first, center_pair.second);

        cv::imwrite(path + std::to_string(++num) + "mask_second_part.png", drawing);
        parte2 = drawing;

        std::cerr << "Second part done.\n";

////////////////////////////////////////////////// THIRD PART /////////////////////////////////////
        /* If a wite pixel is found, run a dfs from it, the result should be the component */
        /* For each component, check what type of ball it is */


        std::cerr << "\nLooking for components...\n";

        std::vector<cv::Mat> ans;
        std::vector<std::vector<cv::Mat>> same_component(balls_color_ranges.size());
        std::vector<std::vector<bool>> vis1(parte1.rows, std::vector<bool>(parte1.cols, 0));
        cv::Mat image_hsv, component;
        cv::cvtColor(image, image_hsv, cv::COLOR_BGR2HSV);
        x = 0;
        path = folderName + "/4-third_part/";

        for(int r = 0; r < parte1.rows; r++){
            for(int c = 0; c < parte1.cols; c++){
                uchar &point = parte1.at<uchar>(r, c);
                if(not vis1[r][c] and point == 255){
                    x++;
                    std::cerr << x << ". Component starting in " << r << ' ' << c << " from part 1";
                    std::map<int, int> type;
                    component = parte1.clone();
                    dfs2(component, r, c, vis1, type, image_hsv);
                    int val = std::max_element(begin(type), end(type), [](const auto &a, const auto &b){
                        return a.second < b.second;
                    })->first;
                    for(int i = 0; i < component.rows; i++){
                        for(int j = 0; j < component.cols; j++){
                            uchar &p = component.at<uchar>(i, j);
                            uchar &p1 = parte1.at<uchar>(i, j);
                            p1 = p == 128? 100 : p1;
                            p = p == 128? 255 : 0;
                        }
                    }
                    std::cerr << " -> dfs done" << '\n';
                    cv::imwrite(path + std::to_string(++num) + "done" + std::to_string(x) +".png", parte1);
                    same_component[val].push_back(component);
                }
                vis1[r][c] = true;
            }
        }

        // create mask of balls that are of the same type
        for(auto &components: same_component){
            cv::Mat temp = cv::Mat::zeros(imgGray.size(), CV_8UC1);
            for(int i = 0; i < components.size(); i++){
                temp += components[i];
            }
            ans.push_back(temp);
        }
        ans.push_back(parte2);

        /////////////////////////////////////////// FINAL RESULT //////////////////////////////////////////////////
        /* Go in each group of components and get the result */
        std::cerr <<"Total " << x << " components and " << ans.size() << " groups\n\n";
        x = 1;

        cv::Mat temp;
        for(auto &item : ans){
            createFolder((folderName + "/5-final_part/Ball" + std::to_string(x++)).c_str());
            path = folderName + "/5-final_part/Ball" + std::to_string(x - 1) + "/";
            cv::imwrite(path + std::to_string(++num) + "mask.png", item);
            temp = item & imgGray;
            cv::imwrite(path + std::to_string(++num) + "gray.png", temp);
            multiply_color(image, temp, item);
            cv::imwrite(path + std::to_string(++num) + "color.png", temp);
            path = folderName + "/5-final_part/all_color/";
            cv::imwrite(path + std::to_string(++num) + "ball" + std::to_string(x - 1) + ".png", temp);
        }

        printf("\nProcessing for \"%s\" has ended.\n", imgName.c_str());
    }
    return 0;
}