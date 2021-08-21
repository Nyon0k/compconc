#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4 // numero de threads

// Variaveis globais
int count = 0;
pthread_mutex_t mutex;
pthread_cond_t condition1;
pthread_cond_t condition2;

// Thread responsavel pela mensagem: Seja bem-vindo!
void* helloMsg(void *arg){
	pthread_mutex_lock(&mutex);
	count++;
	printf("Seja bem-vindo!\n");
	pthread_cond_broadcast(&condition1);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

// Thread responsavel pela mensagem: Fique a vontade!
void* relaxMsg(void *arg){
	pthread_mutex_lock(&mutex);
	if(count==0){
		pthread_cond_wait(&condition1, &mutex);
	}
	printf("Fique a vontade.\n");
	count++;
	if(count==3){
		pthread_cond_signal(&condition2);
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

// Thread responsavel pela mensagem: Sente-se por favor!
void* sitPleaseMsg(void *arg){
	pthread_mutex_lock(&mutex);
	if(count==0){
		pthread_cond_wait(&condition1, &mutex);
	}
	printf("Sente-se por favor.\n");
	count++;
	if(count==3){
		pthread_cond_signal(&condition2);
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

// Thread responsavel pela mensagem: Volte sempre!
void* byeMsg(void *arg){
	pthread_mutex_lock(&mutex);
	if(count<3){
		pthread_cond_wait(&condition2, &mutex);
	}
	printf("Volte sempre!\n");
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main(int argc, char const *argv[]){
    pthread_t t[NTHREADS];

    // Inicia o mutex e as variaveis condicionais
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init (&condition1, NULL);
	pthread_cond_init (&condition2, NULL);

    // Cria threads
    pthread_create(&t[0], NULL, helloMsg, NULL);
    pthread_create(&t[1], NULL, relaxMsg, NULL);
    pthread_create(&t[2], NULL, sitPleaseMsg, NULL);
	pthread_create(&t[3], NULL, byeMsg, NULL);

	// Espera threads finalizarem
    for(int i=1; i<=NTHREADS; i++){
    	pthread_join(t[i-1], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition1);
    pthread_cond_destroy(&condition2);
	return 0;
}
