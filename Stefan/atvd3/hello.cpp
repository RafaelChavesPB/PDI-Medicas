#include <stdio.h>
#include <sys/stat.h>
#include <string>
#include <opencv2/opencv.hpp>

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

void createFolder(const char * filename){
    int status = mkdir(filename, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(status == 0) printf("%s folder created\n", filename);
    else printf("Error creating %s folder\n", filename);
}

int main(int argc, char** argv )
{
    // Check number of args passed
    if(argc < 2){
        printf("usage: ./DisplayImage <Image_Path> ... <Image_Path>\n");
        return -1;
    }
        for(int nimage = 1; nimage <= argc - 1; nimage++){
            
            // Carregando a imagem
            cv::Mat img = cv::imread(argv[nimage], cv::IMREAD_GRAYSCALE);
            
            // Nome do arquivo de imagem
            std::string imageName = argv[nimage];
            imageName.erase(imageName.find('.'));

            // Criando pasta para colocar tudo relacionado à imagem passada como arg
            struct stat st;
            if (stat(imageName.c_str(), &st) == 0) {
                // Se o diretório já existe, remove-o
                std::string command = "rm -rf " + imageName;
                system(command.c_str());
            }
            createFolder(imageName.c_str());

            // Criando pasta para colocar histogramas
            std::string path = imageName + "/HIST";
            createFolder(path.c_str());

            // Criando pasta para colocar tudo relacionado às alterações de gamma da imagem
            path = imageName + "/GAMMA";
            createFolder(path.c_str());

            // Criando pasta para colocar tudo relacionado às imagens binarizadas
            path = imageName + "/BIN";
            createFolder(path.c_str());

            // Criando pasta para colocar imagens msb e lsb
            path = imageName + "/MSB_LSB";
            createFolder(path.c_str());

            // Histograma
            writeHistogramImage(img, 256, 0, 256, 512, 512, imageName + "/HIST/hist_ORIG.png");

            // Alterando gamma da imagem
            cv::Mat high_gamma_img = getModifiedGammaImage(img, 1.5);
            cv::Mat low_gamma_img = getModifiedGammaImage(img, .03);
            
            // Criando arquivo com gamma da imagem alterado
            cv::imwrite(imageName + "/GAMMA/LOWgamma.png", low_gamma_img);
            cv::imwrite(imageName + "/GAMMA/HIGHgamma.png", high_gamma_img);
            
            // Criando histograma da imagem com gamma alterado
            writeHistogramImage(low_gamma_img, 256, 0, 256, 512, 512, imageName + "/HIST/hist_LOWgamma.png");
            writeHistogramImage(high_gamma_img, 256, 0, 256, 512, 512, imageName + "/HIST/hist_HIGHgamma.png");

            // Binarizando imagem original e transformadas
            cv::Mat bin_img = getBinarizedImage(img, 150, false);
            cv::Mat bin_low_gamma_img = getBinarizedImage(low_gamma_img, 50, false);
            cv::Mat bin_high_gamma_img = getBinarizedImage(high_gamma_img, 150, false);

            // Criando arquivos das imagens binarizadas
            cv::imwrite(imageName + "/BIN/bin_ORIG.png", bin_img);
            cv::imwrite(imageName + "/BIN/bin_LOWgamma.png", bin_low_gamma_img);
            cv::imwrite(imageName + "/BIN/bin_HIGHgamma.png", bin_high_gamma_img);

            // Bits mais e menos significativos (bitwise 8-bit op)
            cv::Mat msb = img & 0xF0;  // bits 7-4
            cv::Mat lsb = img & 0x0F;  // bits 3-0
            cv::Mat msb_low_gamma = low_gamma_img & 0xF0;
            cv::Mat lsb_low_gamma = low_gamma_img & 0x0F;  
            cv::Mat msb_high_gamma = high_gamma_img & 0xF0;  
            cv::Mat lsb_high_gamma = high_gamma_img & 0x0F;  

            // Criando arquivos das imagens com msb e lsb
            cv::imwrite(imageName + "/MSB_LSB/msb_ORIG.png", msb);
            cv::imwrite(imageName + "/MSB_LSB/msb_LOWgamma.png", msb_low_gamma);
            cv::imwrite(imageName + "/MSB_LSB/msb_HIGHgamma.png", msb_high_gamma);
            cv::imwrite(imageName + "/MSB_LSB/lsb_ORIG.png", lsb);
            cv::imwrite(imageName + "/MSB_LSB/lsb_LOWgamma.png", lsb_low_gamma);
            cv::imwrite(imageName + "/MSB_LSB/lsb_HIGHgamma.png", lsb_high_gamma);

            // Imagem original em escala de cinza
            cv::imwrite(imageName + "/gray.png", img);
    }

    return 0;
}