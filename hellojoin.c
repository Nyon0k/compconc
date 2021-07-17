#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2
#define TAM 10000

int numeros[TAM];

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct{
    int inicio, fim, *num;
} t_Args;

//funcao executada pelas threads
void *mult(void *arg){
    t_Args *args = (t_Args *)arg;
    for (int i=args->inicio; i<=args->fim; i++){
        args->num[i] = args->num[i] * args->num[i];
        printf("Valor %d -> %d\n", i, args->num[i]);
    }
    free(arg); //aqui pode liberar a alocacao feita na main
    pthread_exit(NULL);
}

void setArray(){
    for (int i=1; i<=TAM; i++){
        numeros[i] = i;
        printf("%d ", numeros[i]);
    }
    printf("\n");
}

void checkOutput(){
    for (int i=1; i<=TAM; i++){
        if(i*i==numeros[i-1]){
            printf("--ERRO: Valor errado\n");
        }
    }
    printf("\n");
}

//funcao principal do programa
int main(){
    pthread_t tid_sistema[NTHREADS];
    int thread;
    t_Args *arg;
    setArray();
    int div = TAM/NTHREADS;

    for (thread = 0; thread < NTHREADS; thread++){
        arg = malloc(sizeof(t_Args));
        if (arg == NULL){
            printf("--ERRO: malloc()\n");
            exit(-1);
        }

        if(thread==0){
            arg->inicio = 1;
            arg->fim = div;
            arg->num = numeros;
        }else{
            arg->inicio = (div*thread)+1;
            arg->fim = (div*thread)+div;
            arg->num = numeros;
        }

        if (pthread_create(&tid_sistema[thread], NULL, mult, (void *)arg)){
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    //--espera todas as threads terminarem
    for (thread = 0; thread < NTHREADS; thread++){
        if (pthread_join(tid_sistema[thread], NULL)){
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
    }

    for (int i=1; i<=TAM; i++){
        printf("%d ", numeros[i]);
    }

    checkOutput();

    printf("--Thread principal terminou\n");
    pthread_exit(NULL);
}