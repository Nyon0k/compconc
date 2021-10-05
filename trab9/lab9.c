#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define T_BUFFER 3
#define CICLOS 15 // Para limitar os prints no console

int nProd;
int nCons;
int contador;
int stop;
int buffer[T_BUFFER];
long limite;
sem_t condProd;
sem_t condCons;

void *produtora(void *args) {
    const long tId = (long) args;
    for (int i = 0; 2; ++i) {
        sem_wait(&condProd);
        if(stop == 1){
            break;
        }
        buffer[limite++] = i;
        printf("P%ld -> %d\n", tId, i);
        if(limite == T_BUFFER){
            sem_post(&condCons);
        }else{
            sem_post(&condProd);
        }
    }
}

void *consumidora(void *args) {
    const long tId = (long) args;

    while (1) {
        sem_wait(&condCons);
        printf("C%ld <- ", tId);
        for (int i = 0; i < T_BUFFER; ++i) {
            printf("%d ", buffer[i]);
        }
        printf("\n");
        limite = 0;
        contador++;
        if(contador == CICLOS){
            printf("--Fim do programa--\n");
            stop = 1;
            sem_post(&condProd);
            break;
        }
        sem_post(&condProd);
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Insira %s <Número Produtores> <Número Consumidores>\n", argv[0]);
        exit(1);
    }
    nProd = atoi(argv[1]);
    nCons = atoi(argv[2]);
    int nThreads = nProd + nCons;
    sem_init(&condProd, 0, 1);
    sem_init(&condCons, 0, 0);
    limite = 0;

    printf("Legenda:    'P' = 'Produtores', '->' = 'Produz', C' = 'Consumidor', '<-' = 'Consome'\n");

    pthread_t tids[nThreads];

    for (long i = 0; i < nProd; ++i) {
        if(pthread_create(&tids[i], NULL, produtora, (void *) i)) {
            printf("ERRO 1\n");
            exit(3);
        }
    }
    for (long i = 0; i < nCons; ++i) {
        if(pthread_create(&tids[nProd+i], NULL, consumidora, (void *) i)) {
            printf("ERRO 2\n");
            exit(3);
        }
    }

    for (int i = 0; i < nThreads; ++i) {
        pthread_join(tids[i], NULL);
    }

    sem_destroy(&condProd);
    sem_destroy(&condCons);
    return 0;
}