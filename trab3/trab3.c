//Soma todos os elementos de um vetor de inteiro
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
float *vetor; //vetor de entrada com dimensao dim
float maiorConc = 0;
float menorConc = 0;

//fluxo das threads
void * tarefa(void * arg){
    long int id = (long int) arg; //identificador da thread
    long int tamBloco = dim/nthreads; //tamanho do bloco de cada thread
    long int ini = id * tamBloco; //elemento inicial do bloco da thread
    long int fim; //elemento final (nao processado) do bloco da thread

    if(id==nthreads-1){
        fim = dim;
    }else{
        fim = ini + tamBloco; //trata o resto se houver
    }

    //acha menor e maior concorrente
    // menorConc = vetor[ini];
    // printf("vetor[ini]: %f\n", vetor[ini]);
    for(long int i=ini; i<fim; i++){
        if(vetor[i]>maiorConc){
            maiorConc = vetor[i];
        }
        if(vetor[i]<menorConc){
            menorConc = vetor[i];
        }
    }

    // printf("menorConc: %f\n", menorConc);
    pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char *argv[]){
    double ini, fim;
    pthread_t *tid; // identificadores das threads no sistema

    //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc<3){
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    //aloca o vetor de entrada
    vetor = (double *) malloc(sizeof(double)*dim);
    if(vetor==NULL){
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    //preenche o vetor de entrada
    for(long int i=0;i<dim; i++){
        vetor[i] = (float) rand()/1000;
        // printf("%f\n", vetor[i]);
    }

    //acha menor e maior sequencial
    float maiorSeq = 0;
    float menorSeq = vetor[0];
    GET_TIME(ini);
    for(long int i=0;i<dim; i++){
        if(vetor[i]>maiorSeq){
            maiorSeq = vetor[i];
        }
        if(vetor[i]<menorSeq){
            menorSeq = vetor[i];
        }
    }
    GET_TIME(fim);
    printf("Tempo sequencial: %lf\n", fim-ini);

    menorConc = vetor[0];

    GET_TIME(ini);
    tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL){
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    //crias as threads
    for(long int i=0; i<nthreads; i++){
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }

    //aguardar o termino das threads
    for(long int i=0; i<nthreads; i++){
        if(pthread_join(*(tid+i), NULL)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    GET_TIME(fim);
    printf("Tempo concorrente: %lf\n", fim-ini);

    //verifica se os valores estao corretos
    
    if(maiorSeq == maiorConc && menorSeq == menorConc){
        printf("SUCESSO!\n");
    }else{
        printf("ERRO -- Resultados diferentes\n");
    }

    //exibir os resultados
    printf("MaiorSeq: %.15f, MenorSeq: %.15f\n", maiorSeq, menorSeq);
    printf("MaiorConc: %.15f, MenorConc: %.15f\n", maiorConc, menorConc);

    //libera as areas
    free(vetor);
    free(tid);

    return 0;
}