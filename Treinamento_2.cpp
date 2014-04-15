//Código que realiza o treinamento utilizando RNA do OpenCV. Código ainda se encontra incompleto!
//Última atualização - 15/04/2014 10h40
//_______________________________________________________________________________________________

//#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/ml/ml.hpp"
#include <stdio.h>
#include <fstream>

using namespace std;
using namespace cv;

//-------------------------Declaração de algumas constantes-----------------------------

#define TRAINING_SAMPLES 320		//Num. de amostras para treinamento
#define ATTRIBUTES 9			//Num. de atributos dos testes
#define TEST_SAMPLES 11			//Num. de amostras reais para teste da RNA
#define CLASSES 2			//Num. de classes separadas para treinar a RNA
//--------------------------------------------------------------------------------------

//----------------Essa função abre os arquivos de treinamento e teste e converte as informações em 2 matrizes.--------------
//---------------- A matriz de classe possui 4 classes, uma coluna por cada classe.
//--------------------------------------------------------------------------------------------------------------------------

  void read_dataset (const char *filename, Mat &data, Mat &classes, int total_samples)
  {
    int label;
    float pixelvalue;
    //abre o arquivo para treinamento ('r' é um parâmetro apenas de leitura)
    FILE* inputfile = fopen(filename , "r" );
 
    //percorre cada linha do arquivo
   for(int row = 0; row < total_samples; row++)
   {
       //percorre cada abributo presente nas linhas
     for(int col = 0; col <=ATTRIBUTES; col++)
        {
            //pega cada valor do "pixel" do arquivo
            if (col <= ATTRIBUTES){
 
                fscanf(inputfile, "%f ", &pixelvalue);
                data.at<float>(row,col) = pixelvalue;
 
            }//se conferir
            //else if (col == ATTRIBUTES){
                //define o valor do rótulo como 1
              //  fscanf(inputfile, "%i", &label);
                //classes.at<float>(row,label) = 1.0;
 
            //}
        }
    }
 
    fclose(inputfile);
 
}

//----------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------Main do programa------------------------------------------------------------

int main( int argc, char *argv[] )
{
    //matriz para armazenar os samples de treinamento
    cv::Mat training_set(TRAINING_SAMPLES,ATTRIBUTES,CV_32F);
    //matriz pra armazenar os marcadores nos treinamentos
    cv::Mat training_set_classifications(TRAINING_SAMPLES, CLASSES, CV_32F);
    //matriz para armazenar os samples de teste
    cv::Mat test_set(TEST_SAMPLES,ATTRIBUTES,CV_32F);
    //matriz para armazenar os marcadores dos testes	
    cv::Mat test_set_classifications(TEST_SAMPLES,CLASSES,CV_32F);
 
    //Armazena o resultado da classificação
    cv::Mat classificationResult(1, CLASSES, CV_32F);
    //carrega os arquivos de treinamento e teste
    read_dataset("x.txt", training_set, training_set_classifications, TRAINING_SAMPLES);
    //home/dunric/'Área de Trabalho'/Dados/RNA/Treinamento_2/
    read_dataset("teste.txt", test_set, test_set_classifications, TEST_SAMPLES);
 
        // Define a estrutura da RNA (parâmetros)
        // A RNA possui 3 camadas
        // - 1 neurônio de entrada
        // - 16 neurônios na camada escondida
        // - 4 saídas, uma para cada classe
 
        cv::Mat layers(3,1,CV_32S);
        layers.at<int>(0,0) = ATTRIBUTES;//camada de entrada
        layers.at<int>(1,0)=5;//camada escondida
        layers.at<int>(2,0) =CLASSES;//camada saída
 
        //criando a RNA
        CvANN_MLP nnetwork(layers, CvANN_MLP::SIGMOID_SYM,0.6,1); //utiliza sigmódide
 
        CvANN_MLP_TrainParams params(                                  
 
                                        // termina o treinamento após 1000 interações
                                        // ou uma mudança muito pequena no ajuste
                                        // dos pesos da RNA
                                        cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001),
                                        // usa backpropogation para treinar
                                        CvANN_MLP_TrainParams::BACKPROP,
                                        // coeficientes para o treinamento 'backpropogation' utilizando
                                        // padrões da documentação
                                        0.1,
                                        0.1);
 
        // treinando a RNA (Usando arquivos)
 
        printf( "\nUsando arquivos de treinamento\n");
        int iterations = nnetwork.train(training_set, training_set_classifications,cv::Mat(),cv::Mat(),params);
        printf( "Iterações no treinamento: %i\n\n", iterations);
 
        // Salva o modelo gerado em um arquivo xml
        CvFileStorage* storage = cvOpenFileStorage( "param.txt", 0, CV_STORAGE_WRITE );
        nnetwork.write(storage,"DigitOCR");
        cvReleaseFileStorage(&storage);




 return 0;
 
}























