//Código em C calcula a media, variância e desvio padrão de amostras de 10 imagens e salva as informações em .txt
//
//Execução no terminal:       gcc -o media_2 media_2.cpp `pkg-config opencv --cflags --libs`

#include <iostream>
#include "opencv2/opencv.hpp"
#include <stdio.h>


using namespace std;
using namespace cv;

int main (int argc, char **argv)
{
//---------------------------Cria Arquivo para leitura e gravação----------------------------
FILE *arq;
  int i;
  float result;
  //clrscr();
  arq = fopen("x.txt", "w+");  // Cria um arquivo texto para gravação
  if (arq == NULL) // Se não conseguiu criar
  {
     printf("Problemas na CRIACAO do arquivo\n");
     return 1;
  }

//------------->>>>>>>>>>>>>>>> Altere o valor de Z para de acordo com o número de imagens a ser testada <<<<<<<<<<<----------
  char nome[320];
  for (int z=0; z<10; z++){	//esquema para abrir imagem dentro de um laço 'for'. Alterar o valor Z = nº imagens
  sprintf(nome, "%d.jpg", z);
  
  string src=nome;
  IplImage* img=cvLoadImage(src.c_str()); //cria uma estrutura de imagem 'img' e salva a imagem 'src' nela
  unsigned char* corData=(unsigned char*) img->imageData; //gera ponteiro para manipular as cores

//-----------------------------calcula a média das cores de uma imagem---------------------------------------------
//cria variáveis para auxiliar no cálculo
	float mediaRed,mediaGreen,mediaBlue;
	mediaRed=mediaGreen=mediaBlue=0.f;

//percorre cada pixel da imagem e realiza a soma
	for(int i=0;i<img->width*img->height*3;i+=3)
	{
	  mediaBlue +=corData[1];
	  mediaGreen +=corData[i+1];
	  mediaRed +=corData[i+2];
	}
//calcula a média
	mediaBlue /=img->width*img->height;
	mediaGreen /=img->width*img->height;
	mediaRed /=img->width*img->height;
//exibe na tela os resultados obtidos
printf ("+---------------------------+\n");
printf("Media Canal Azul:%f\n",mediaBlue);
printf("Media Canal Verde:%f\n",mediaGreen);
printf("Media Canal Vermelho:%f\n",mediaRed);
printf ("+---------------------------+\n \n");

//---------------------------------------------------------------------------------------------------------------
//---------------------------------calcula a variância de cores da imagem------------------------------------------
  float varianciaRed, varianciaGreen,varianciaBlue;  //cria as variáveis
  varianciaRed=varianciaGreen=varianciaBlue=0.f;

  for(int d=0;d<img->width*img->height*3;d+=3)	//for para percorrer cada valor dos pixels
  {
	varianciaBlue +=(corData[d]-mediaBlue)*(corData[d]-mediaBlue);
	varianciaGreen +=(corData[d+1]-mediaGreen)*(corData[d+1]-mediaGreen);
	varianciaRed +=(corData[d+2]-mediaRed)*(corData[d+2]-mediaRed);
  }
//calcula a variancia
  varianciaBlue /=img->width*img->height;
  varianciaGreen /=img->width*img->height;
  varianciaRed /=img->width*img->height;

//exibe na tela resultados obtidos
printf ("+---------------------------+\n");
printf("Variancia Canal Azul:%f\n",varianciaBlue);
printf("Variancia Canal Verde:%f\n",varianciaGreen);
printf("Variancia Canal Vermelho:%f\n",varianciaRed);
printf ("+---------------------------+\n \n");

//--------------------------------------Calcula o desvio padrão--------------------------------------------------
  float dev_Blue, dev_Green,dev_Red;
  dev_Blue = sqrt(varianciaBlue);		//raiz quadrada
  dev_Green = sqrt(varianciaGreen);
  dev_Red = sqrt(varianciaRed);

//exibe na tela resultados obtidos
printf ("+---------------------------+\n");
printf("Desvio Padrao Canal Azul:%f\n",dev_Blue);
printf("Desvio Padrao Canal Verde:%f\n",dev_Green);
printf("Desvio Padrao Canal Vermelho:%f\n",dev_Red);
printf ("+---------------------------+\n \n");

printf ("+---------------------------+\n");
printf ("Dados salvos em 'x.txt'\n");
printf ("+---------------------------+\n");

//---------------------------------------------------------------------------------------------------------------





//--------------------------Salva informações HD-----------------------------------------------------------------
  
  
// A funcao 'fprintf' devolve o número de bytes gravados 
// ou EOF se houve erro na gravação
      result = fprintf(arq,"%f %f %f %f %f %f %f %f %f\n",mediaBlue,mediaGreen,mediaRed,varianciaBlue,varianciaGreen,varianciaRed,dev_Blue,dev_Green,dev_Red );  					  
      if (result == EOF)		    
	  printf("Erro na Gravacao\n");
  


//---------------------------------------------------------------------------------------------------------------
  cvWaitKey('q');
  cvReleaseImage(&img);

}
  fclose(arq);			//Fecha arquivo de leitura
  return 0;
}


