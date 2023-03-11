#include "helpers.h"

void clearString(std::string &s){
    for(int i = (int)s.size() - 1; i >= 0; i--){
        if(s[i] == '.'){
            s.erase(i);
            break;
        }
    }
    std::string x = "";
    for(int i = (int)s.size() - 1; i >= 0; i--){
        if(s[i] == '/'){
            for(int j = i + 1; j < (int)s.size(); j++)
                x += s[j];
            break;
        }
    }
    s = x;
}

void createFolder(const char * filename){
    struct stat st;
    if (stat(filename, &st) == 0) {
        // Se o diretório já existe, remove-o
        std::string command = "rm -rf " + std::string(filename);
        system(command.c_str());
    }
    int status = mkdir(filename, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(status == 0) printf("%s folder created\n", filename);
    else printf("Error creating %s folder\n", filename);
}

cv::Mat getModifiedGammaImage(cv::Mat &img, double gamma){
        // Matriz temporária do mesmo tamanho da imagem
        cv::Mat gamma_image = cv::Mat::zeros(img.size(), img.type());

        // Criando a tabela de pesquisa (LUT)
        cv::Mat lut_gamma(1, 256, CV_8UC1);

        for(int i = 0; i < 256; i++)
            lut_gamma.at<uchar>(i) = cv::saturate_cast<uchar>(cv::pow((double)i / 255.0, gamma) * 255.0);

        // Aplicando a transformação usando (LUT) ao canal único (escala de cinza)
        cv::LUT(img, lut_gamma, gamma_image);

        return gamma_image;
}


void writeHistogramImage(cv::Mat &img, int histSize, float low, float high, int width, int height, std::string filename){     
        // Definindo os valores mínimo e máximo para o histograma
        float range[2] = {low, high};
        const float* histRange = { range };

        // Calculando o histograma
        cv::Mat hist;
        cv::calcHist(&img, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

        // Criando uma imagem em branco para desenhar o histograma
        int binWidth = cvRound((double) width / histSize);
        cv::Mat histImage(height, width, CV_8UC1, cv::Scalar(255));

        // Normalizando o histograma para o tamanho da imagem criada
        cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

        // Desenhando o histograma na imagem em branco
        for(int i = 0; i < histSize; i++) {
            cv::line(histImage, 
                cv::Point(binWidth * i, histImage.rows),
                cv::Point(binWidth * i, histImage.rows - cvRound(hist.at<float>(i))),
                cv::Scalar(0), (double)width / histSize, 8, 0);
        }

        // Criando arquivo com a imagem do o histograma
        cv::imwrite(filename, histImage);
}

cv::Mat getBinarizedImage(cv::Mat &img, int threshold, bool inv){
    cv::Mat binary_img;

    if(inv)
        cv::threshold(img, binary_img, threshold, 255, cv::THRESH_TOZERO_INV);   
    else
        cv::threshold(img, binary_img, threshold, 255, cv::THRESH_TOZERO);
    
    return binary_img;
}

