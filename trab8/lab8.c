#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

// Variaveis globais
sem_t condt1, condt2, condt3;

void *t1 (void *arg) {
    sem_wait(&condt1);
    printf("Fique a vontade.\n");
    sem_wait(&condt1);
    sem_post(&condt3);
    pthread_exit(NULL);
}

void *t2 (void *arg) {
    printf("Seja bem-vindo!\n");
    sem_post(&condt1);
    sem_post(&condt2);
    pthread_exit(NULL);
}

void *t3 (void *arg) {
    sem_wait(&condt3);
    printf("Volte sempre!\n");
    pthread_exit(NULL);
}

void *t4 (void *arg) {
    sem_wait(&condt2);
    printf("Sente-se por favor.\n");
    sem_post(&condt2);
    sem_post(&condt3);
    pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];

    //inicia os semaforos
    sem_init(&condt1, 0, 0);
    sem_init(&condt2, 0, 0);
    sem_init(&condt3, 0, 0);

    //cria as tres threads
    if (pthread_create(&tid[0], NULL, t1, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
    if (pthread_create(&tid[1], NULL, t2, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
    if (pthread_create(&tid[2], NULL, t3, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
    if (pthread_create(&tid[3], NULL, t4, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }

    for (int t=0; t<NTHREADS; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        }
    } 
    
    if(sem_destroy(&condt1)) {
		printf("ERRO\n");
		exit(-1);
	}
	if(sem_destroy(&condt2)) {
		printf("ERRO\n");
		exit(-1);
	}
	if(sem_destroy(&condt3)) {
		printf("ERRO\n");
		exit(-1);
	}

    return 0;
}