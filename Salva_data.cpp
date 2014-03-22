//Código recorta uma região de interesse de 55x55 pixels e salva o histograma juntamente com as informações em .txt
//Execução no terminal:       gcc -o Salva_data Salva_data.cpp `pkg-config opencv --cflags --libs`
//			      ./Salva_data imagem.jpg

#include <iostream>
#include "opencv2/opencv.hpp"
#include <stdio.h>

using namespace std;
using namespace cv;


Mat src,img,ROI;  //Cria matrizes 'src', 'img', 'ROI' ('Mat' na verdade é uma struct já existente na biblioteca)
Rect Retangulo(0,0,0,0); //Armazena as dimensões do 'Retangulo' [ Rect xxx( x, y, width, height)...onde x: coordenada canto superior esq.
			//y: coordenada canto inferior esq., width: largura ret.; height: altura ret.


 Point P1(0,0);	//Cria 'P1' e 'P2' utilizando a estrutura 'Point' que irá salvar coordenadas
 Point P2(0,0);

//Cria algumas constantes que serão utilizadas no programa
const char* winName="Corte Imagem";	//WinName guarda o nome Corte Imagem
bool clicked=false;			//booleano
int a=0;				//variável usada para nomear as imagens salvas
char imgName[15];
char nome[15];


//Essa função limita o retângulo criado nas dimensões 33x33 pixel
void Limita(){
       //Compara, se o valor de Retangulo.width (largura) for maior que 33 pixels 
       if(Retangulo.width>55) //caso seja verdade, os valores são igualados a 33 pixel
         Retangulo.width=55;

	//Compara, se o valor de Retangulo.height (altura) for maior que 33 pixels
       if(Retangulo.height>55) //caso seja verdade, os valores são igualados a 33 pixels
         Retangulo.height=55;

	//Evita que o valor do canto superior esq. seja inferior que 0, oque é impossivel de acontecer
        if(Retangulo.x<0)
         Retangulo.x=0;	//Se a condição for verdadeira o valor é igualado a 0
	
	//Evita que o valor do canto inferior esq. seja inferior que 0, oque é impossivel
       if(Retangulo.y<0)
         Retangulo.height=0;	//Se a condição for verdadeira o valor é igualado a 0
}

//Essa função é responsável por manipular o arquivo da imagem cortada
void showImage(){
    img=src.clone();	//A imagem salva na matriz 2D 'src' é clonada e salva na matriz img
    Limita();	//A função de checagem é chamada aqui (Obs.: repare que a matriz utilizada na função é justamente a 'img' (imagem man.)
    if(Retangulo.width>0&&Retangulo.height>0){	//se a altura e largura da imagem em 'Retangulo' for maior q 0 essas coordenadas extraidas...
        ROI=src(Retangulo);			//...de 'src' será salva na matriz ROI
        imshow("Corte",ROI);	//Apresenta imagem cortada (região interesse)
    }

    rectangle(img, Retangulo, Scalar(0,255,0), 1, 8, 0 );  // Função desenha o triangulo verde...img=imagem; Retangulo=dimensões do retângulo;
    							  //Scalar(0,255,0)= cor do triângulo; 1,8,0=padrão
//imshow("winName",img);  //Essa função exibe a imagem de trabalho no tamanho original. É opcional isso.				  
}


void onMouse( int event, int x, int y, int f, void* ){	//Função captura dados mouse


    switch(event){

        case  CV_EVENT_LBUTTONDOWN  :
                                        clicked=true;

                                        P1.x=x;
                                        P1.y=y;
                                        P2.x=x;
                                        P2.y=y;
                                        break;

        case  CV_EVENT_LBUTTONUP    :
                                        P2.x=x;
                                        P2.y=y;
                                        clicked=false;
                                        break;

        case  CV_EVENT_MOUSEMOVE    :
                                        if(clicked){
                                        P2.x=x;
                                        P2.y=y;
                                        }
                                        break;

        default                     :   break;


    }


    if(clicked){
     if(P1.x>P2.x){ Retangulo.x=P2.x;
                       Retangulo.width=P1.x-P2.x; }
        else {         Retangulo.x=P1.x;
                       Retangulo.width=P2.x-P1.x; }

        if(P1.y>P2.y){ Retangulo.y=P2.y;
                       Retangulo.height=P1.y-P2.y; }
        else {         Retangulo.y=P1.y;
                       Retangulo.height=P2.y-P1.y; }

    }


showImage(); //Mostra a imagem resultante

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Histograma(Mat, int){
Mat dst; //variáveis para manipulação da imagem
int i=0;
char imgName[15];
  //Separa os canais B,G e R da imagem 'ROI' e guarda essas informações em um vetor
  vector<Mat> bgr_planos;	//cria o vetor com o nome 'bgr_planos'
  split(ROI, bgr_planos);	//usa a função 'split' para separar os planos e salvar em 'bgr_planos'

  //limita o numero de binarios (equivale ao numero de tons das cores 0~255)
  int histSize=256;

  //Seta os limites para cada canal B G R
  float range[]={0,256};	//cria um vetor com os valores
  const float* histRange={range};	//iguala histRange aos valores definidos na linha anterior
  
  //Essa parte não entendi nada
  bool uniform=true ;bool accumulate=false;
  ///////////////////////////////

  //Cria 3 vetores para salvar os dados de cada plano do histograma gerado ****************
  Mat B_hist, G_hist, R_hist;

  //Calcula o histograma por cada plano
  calcHist (&bgr_planos[0], 1, 0, Mat(), B_hist, 1, &histSize, &histRange, uniform, accumulate);
  calcHist (&bgr_planos[1], 1, 0, Mat(), G_hist, 1, &histSize, &histRange, uniform, accumulate);
  calcHist (&bgr_planos[2], 1, 0, Mat(), R_hist, 1, &histSize, &histRange, uniform, accumulate);

  //Cria uma estrutura de imagem para exibir o histograma
  int hist_w=512; int hist_h=400;	//dimensões da estrutura
  int bin_w=cvRound ((double) hist_w/histSize);  //acredito que calcula a largura binária

  Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0,0,0)); //a estrutura em si com os parâmetros

  //Não entendi nada também
  normalize(B_hist, B_hist,0,histImage.rows, NORM_MINMAX, -1, Mat());
  normalize(G_hist, G_hist,0,histImage.rows, NORM_MINMAX, -1, Mat());
  normalize(R_hist, R_hist,0,histImage.rows, NORM_MINMAX, -1, Mat());

  //Desenhando o histograma por cada canal
  for( int i = 1; i < histSize; i++ )  //Cria uma linha ponto a ponto de 0 até 255, utilizando a cor do canal específico
{
    line( histImage, Point( bin_w*(i-1), hist_h - cvRound(B_hist.at<float>(i-1)) ) ,
                     Point( bin_w*(i), hist_h - cvRound(B_hist.at<float>(i)) ),
                     Scalar( 255, 0, 0), 2, 8, 0  );	//Usa a cor 'Blue'
    line( histImage, Point( bin_w*(i-1), hist_h - cvRound(G_hist.at<float>(i-1)) ) ,
                     Point( bin_w*(i), hist_h - cvRound(G_hist.at<float>(i)) ),
                     Scalar( 0, 255, 0), 2, 8, 0  );	//Usa a cor 'Green'
    line( histImage, Point( bin_w*(i-1), hist_h - cvRound(R_hist.at<float>(i-1)) ) ,
                     Point( bin_w*(i), hist_h - cvRound(R_hist.at<float>(i)) ),
                     Scalar( 0, 0, 255), 2, 8, 0  );	//Usa a cor 'Red'
}


      

//________________________________________________
////Salva a imagem do histograma e as informações no HD (.YML)

  sprintf(imgName,"%d.jpg", a);
  imwrite(imgName, histImage);
  cout<<"  Salvo "<<imgName<<endl;
  sprintf(nome,"%d.txt", a);
  FileStorage fs(nome,FileStorage::WRITE);
  if (!fs.isOpened()) {return;}
  fs << "Hist_Data" << histImage;
  fs.release();



//________________________________________________

}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main( int argc, char** argv)  //MAIN
{

    cout<<"Selecione a regiao de interesse de 55x55 pixels"<<endl<<endl;
    
    cout<<"------> Pressione 'h' para salvar o histograma"<<endl<<endl;

    cout<<"------> Pressione 'r' para resetar"<<endl;
    cout<<"------> Pressione 'Esc' para finalizar"<<endl<<endl;


    src=imread(argv[1],1);		//Carrega a imagem desejada
    if (!src.data){
	return -1;}

    namedWindow(winName,WINDOW_NORMAL);		//Cria uma janela de imagem (no caso de nome 'winName= Corte Imagem')
    setMouseCallback(winName,onMouse,NULL );	//chama a função SetMouseCallback que aplica a função onMouse na janela desejada...
						//...no caso a janela 'winName'
    imshow(winName,src);			//Apresenta a janela winName exibindo a imagem src

    while(1){			//Salva a imagem gerada com um nome numérico
    char c=waitKey();
    
    if(c==27) break;		//Fecha o programa
    if(c=='r') {Retangulo.x=0;Retangulo.y=0;Retangulo.width=0;Retangulo.height=0;}	//Reseta a imagem
    showImage();
    if(c=='h'){
    Histograma(ROI,a);
      a++;
      
}
    }


    return 0;
}
