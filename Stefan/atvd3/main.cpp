#include "../helpers/helpers.h"

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
            clearString(imageName);
            std::string folderName = "Results-IMG[" + imageName + "]";
            
            // Criando pasta para colocar resultados relacionados a img
            createFolder(folderName.c_str());

            // Criando pasta para colocar histogramas
            std::string path = folderName + "/HIST";
            createFolder(path.c_str());

            // Criando pasta para colocar tudo relacionado às alterações de gamma da imagem
            path = folderName + "/GAMMA";
            createFolder(path.c_str());

            // Criando pasta para colocar tudo relacionado às imagens binarizadas
            path = folderName + "/BIN";
            createFolder(path.c_str());

            // Criando pasta para colocar imagens msb e lsb
            path = folderName + "/MSB_LSB";
            createFolder(path.c_str());

            // Histograma
            writeHistogramImage(img, 256, 0, 256, 512, 512, folderName + "/HIST/hist_ORIG.png");

            // Alterando gamma da imagem
            cv::Mat high_gamma_img = getModifiedGammaImage(img, 1.5);
            cv::Mat low_gamma_img = getModifiedGammaImage(img, .03);
            
            // Criando arquivo com gamma da imagem alterado
            cv::imwrite(folderName + "/GAMMA/LOWgamma.png", low_gamma_img);
            cv::imwrite(folderName + "/GAMMA/HIGHgamma.png", high_gamma_img);
            
            // Criando histograma da imagem com gamma alterado
            writeHistogramImage(low_gamma_img, 256, 0, 256, 512, 512, folderName + "/HIST/hist_LOWgamma.png");
            writeHistogramImage(high_gamma_img, 256, 0, 256, 512, 512, folderName + "/HIST/hist_HIGHgamma.png");

            // Binarizando imagem original e transformadas
            cv::Mat bin_img = getBinarizedImage(img, 150, false);
            cv::Mat bin_low_gamma_img = getBinarizedImage(low_gamma_img, 50, false);
            cv::Mat bin_high_gamma_img = getBinarizedImage(high_gamma_img, 150, false);

            // Criando arquivos das imagens binarizadas
            cv::imwrite(folderName + "/BIN/bin_ORIG.png", bin_img);
            cv::imwrite(folderName + "/BIN/bin_LOWgamma.png", bin_low_gamma_img);
            cv::imwrite(folderName + "/BIN/bin_HIGHgamma.png", bin_high_gamma_img);

            // Bits mais e menos significativos (bitwise 8-bit op)
            cv::Mat msb = img & 0xF0;  // bits 7-4
            cv::Mat lsb = img & 0x0F;  // bits 3-0
            cv::Mat msb_low_gamma = low_gamma_img & 0xF0;
            cv::Mat lsb_low_gamma = low_gamma_img & 0x0F;  
            cv::Mat msb_high_gamma = high_gamma_img & 0xF0;  
            cv::Mat lsb_high_gamma = high_gamma_img & 0x0F;  

            // Criando arquivos das imagens com msb e lsb
            cv::imwrite(folderName + "/MSB_LSB/msb_ORIG.png", msb);
            cv::imwrite(folderName + "/MSB_LSB/msb_LOWgamma.png", msb_low_gamma);
            cv::imwrite(folderName + "/MSB_LSB/msb_HIGHgamma.png", msb_high_gamma);
            cv::imwrite(folderName + "/MSB_LSB/lsb_ORIG.png", lsb);
            cv::imwrite(folderName + "/MSB_LSB/lsb_LOWgamma.png", lsb_low_gamma);
            cv::imwrite(folderName + "/MSB_LSB/lsb_HIGHgamma.png", lsb_high_gamma);

            // Imagem original em escala de cinza
            cv::imwrite(folderName + "/gray.png", img);
    }

    return 0;
}