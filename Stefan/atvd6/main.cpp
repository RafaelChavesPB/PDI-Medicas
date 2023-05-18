#include "../helpers/helpers.h"

cv::MorphShapes getStructureElemenType(int num){
    if(num == 1) return cv::MORPH_RECT;
    else if(num == 2) return cv::MORPH_ELLIPSE;
    else return cv::MORPH_CROSS;
}

int askStructuredElementType(){
    int tipo;
    printf("\nDigite o tipo de elemento estruturante.\n1 -> Retangulo\n2 -> Elipse\n3+ -> Cruz\nInput: ");
    scanf("%d", &tipo);
    if(tipo > 3 or tipo < 1) tipo = 3;
    return tipo;
}

int* askStructuredElementShape(){
    int* ans = (int*) malloc(2 * sizeof(int));
    printf("\nDigite o tamanho da matriz do elemento estruturante.\nLinhas: ");
    scanf("%d", &ans[0]);
    printf("Colunas: ");
    scanf("%d", &ans[1]);
    printf("\n");
    return ans;
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
        

        // Gerando os resultados
        path = folderName + "/";
        cv::imwrite(path + "original.png", image);
        cv::imwrite(path + "original_grayscale.png", imgGray);

        int option = 0;
        printf("\nDigite qual operacao realizar:\n1 -> Dilatacao\n2 -> Erosao\n3+ -> Ambas\nInput: ");
        scanf("%d", &option);
        if(option > 3 or option < 1) option = 3;
        std::string choice[3] = {"Dilatacao", "Erosao", "Ambas"};
        printf("Operacao escolhida -> %s\n", choice[option - 1].c_str());

        std::string se_type[3] = {"Retangulo", "Elipse", "Cruz"};
        int tipo = askStructuredElementType();
        int* x = askStructuredElementShape();
        int lin = x[0];
        int col = x[1];
        free(x);
        printf("Elemento estruturante -> %s. Size -> %d x %d\n", se_type[tipo - 1].c_str(), lin, col);

        cv::Mat dilated_image, eroded_image;
        cv::Mat dilate_element = cv::getStructuringElement(getStructureElemenType(tipo), cv::Size(lin, col));
        cv:: Mat erode_element = cv::getStructuringElement(getStructureElemenType(tipo), cv::Size(lin, col));

        if(option == 1){
            cv::dilate(image, dilated_image, dilate_element);
        }else if(option == 2){
            cv::erode(image, eroded_image, erode_element);
        }else{ 
            cv::dilate(image, dilated_image, dilate_element);
            cv::erode(image, eroded_image, erode_element);
        }

        path = folderName + "/DILATACAO";
        createFolder(path.c_str());
        if(option == 1 or option == 3)
            cv::imwrite(path + "/" + choice[0] + se_type[tipo - 1] + ".png", dilated_image);

        path = folderName + "/EROSAO";
        createFolder(path.c_str());

        if(option >= 2)
            cv::imwrite(path + "/" + choice[1] + se_type[tipo - 1] + ".png", eroded_image);
        
        printf("Processed image \"%s\"\n", imgName.c_str());
    }
    return 0;
}