#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H

#include <sys/time.h>
#define BILLION 1000000000L

#define GET_TIME(now) { \
   struct timespec time; \
   clock_gettime(CLOCK_MONOTONIC, &time); \
   now = time.tv_sec + time.tv_nsec/1000000000.0; \
}
#endif

float *leMatrizBinario(char *arquivo, int *linhasX, int *colunasX){
   float *matriz; //matriz que será carregada do arquivo
   int linhas, colunas; //dimensoes da matriz
   long long int tam; //qtde de elementos na matriz
   FILE * descritorArquivo; //descritor do arquivo de entrada
   size_t ret; //retorno da funcao de leitura no arquivo de entrada
   
  /* //recebe os argumentos de entrada
   if(argc < 2) {
      fprintf(stderr, "Digite: %s <arquivo entrada>\n", argv[0]);
      return 1;
   } */
   
   //abre o arquivo para leitura binaria
   descritorArquivo = fopen(arquivo, "rb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return NULL;
   }

   //le as dimensoes da matriz
   ret = fread(&linhas, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
      return NULL;
   }
   ret = fread(&colunas, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
      return NULL;
   }
   tam = linhas * colunas; //calcula a qtde de elementos da matriz
   *linhasX = linhas;
   *colunasX = colunas;

   //aloca memoria para a matriz
   matriz = (float*) malloc(sizeof(float) * tam);
   if(!matriz) {
      fprintf(stderr, "Erro de alocao da memoria da matriz\n");
      return NULL;
   }

   //carrega a matriz de elementos do tipo float do arquivo
   ret = fread(matriz, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
      return NULL;
   }
  
   /*//imprime a matriz na saida padrao
   for(int i=0; i<linhas; i++) { 
      for(int j=0; j<colunas; j++)
        fprintf(stdout, "%.6f ", matriz[i*colunas+j]);
      fprintf(stdout, "\n");
   }*/

   
   //finaliza o uso das variaveis
   fclose(descritorArquivo);
   
   return matriz;
}

int geraMatrizBinario(char *arquivo, float* matriz, int linhas, int colunas) {
   
   long long int tam; //qtde de elementos na matriz
   FILE * descritorArquivo; //descritor do arquivo de saida
   size_t ret; //retorno da funcao de escrita no arquivo de saida
   
   tam = linhas * colunas;

   //escreve a matriz no arquivo
   //abre o arquivo para escrita binaria
   descritorArquivo = fopen(arquivo, "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   //escreve numero de linhas e de colunas
   ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo);
   //escreve os elementos da matriz
   ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo);
   ret = fwrite(matriz, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
   }

   //finaliza o uso das variaveis
   fclose(descritorArquivo);
   return 0;
}

int main(int argc, char* argv[]){
  float *matrizA;//mat carregada do arquivo1
  float *matrizB;//mat carregada do arquivo2
  int linhasA, colunasA; //dimensoes da matrizA
  int linhasB, colunasB; //dimensoes da matrizB	
  double start, finish, elapsed;

	//recebe os argumentos de entrada

  if (argc < 3) {
        fprintf(stderr, "Digite: %s <arquivo da matriz A> <arquivo da matriz B> <arquivo da matriz de Saída>\n", argv[0]);
        return 1;
    }
	// começando o 1º timer
  
  GET_TIME(start);

  char *arquivoA = argv[1];
  char *arquivoB = argv[2];
  char *arquivoC = argv[3];

  
  matrizA = leMatrizBinario(arquivoA, &linhasA, &colunasA);
  matrizB = leMatrizBinario(arquivoB, &linhasB, &colunasB);
  
  if (matrizA == NULL || matrizB == NULL){
        fprintf(stderr, "Erro nas Matrizes\n");
        return 0;
  }

	//verificando a compatibilidade das matrizes
 
  if (colunasA != linhasB){
        fprintf(stderr, "Matriz A tem o número de colunas diferente do número de linhas da Matriz B!!!\n");
        return 0;
    }

	//Matriz Resultado
  	
	//dimensao
  int linhasC = linhasA;
  int colunasC = colunasB;
  int tamC = linhasC * colunasC;

	//alocação de memória
  float *matrizC = (float*) malloc(sizeof(float)*tamC);  

	//fim do 1º timer

  GET_TIME(finish);
  elapsed = start-finish;
  printf("Tempo de inicialização %lf", elapsed);


	// começando o 2º timer
  
  GET_TIME(start);
	
	//multiplicação das matrizes e preenchimento da matrizC

  float soma;
  for (int i = 0; i < linhasC; i++){
      for (int j = 0; j < colunasC; j++){
          soma = 0;
          for (int n = 0; n < colunasA; n++){                
              soma += matrizA[i*colunasA + n] * matrizB[n* colunasB + j];
          }
          matrizC[i*colunasC + j] = soma;            
      }
  }

	//fim do 2º timer

  GET_TIME(finish);
  elapsed = start-finish;
  printf("Tempo de processamento%lf", elapsed);

	// começando o 3º timer
  
  GET_TIME(start);

  	//gerando arquivo binário

  geraMatrizBinario(arquivoC, matrizC, linhasC, colunasC);


	//liberação da memória

  free(matrizA);
  free(matrizB);
  free(matrizC);

	//fim do 3º timer

  GET_TIME(finish);
  elapsed = start-finish;
  printf("Tempo de finalização%lf", elapsed);

	
  return 0;
}
