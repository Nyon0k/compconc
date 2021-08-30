#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include "timer.h"

int nthreads; //numero de threads
long long int numero; //numero que deseja achar os divisores
long long int quantDivisores = 0; //quantidade de divisores
long long int divSequencial = 0; //quantidade de divisores algoritmo sequencial
pthread_mutex_t lock; //variavel usada na exclusao mutua por escalonamento

//struct para retornar tamanho do vetor com os divisores
typedef struct retorno {
	long long int tam;
	long long int* divisores;
} retorno;

//struct para retornar tamanho do vetor com os divisores sequencialmente
typedef struct retornoSeq {
	long long int tamSeq;
	long long int* divisoresSeq;
} retornoSeq;

//encontra os divisores do numero
void* procurarDivisores(void* arg) {
	long long int id = (long long int) arg; //identificador local da thread
	retorno* divisoresBloco = (retorno*) malloc(sizeof(retorno)); //struct de retorno de cada thread
	if(divisoresBloco == NULL) {
		puts("ERRO - malloc - divisoresBloco");
		exit(1);
	}
	divisoresBloco->divisores = (long long int*) malloc(sizeof(long long int)); //divisores que cada thread calculou
	if(divisoresBloco->divisores == NULL) {
		puts("ERRO - malloc - divisoresBloco->divisores");
		exit(1);
	}
	divisoresBloco->tam = 0; //numero de divisores calculado por cada thread
	for(long long int i = id; i <= ((numero)^(1/2)); i+=nthreads) {
		if(numero % i == 0) {
			divisoresBloco->divisores[divisoresBloco->tam] = i;
			divisoresBloco->tam++;
			//realoca memoria do vetor de divisores
			divisoresBloco->divisores = (long long int*) realloc(divisoresBloco->divisores, (divisoresBloco->tam + 1)*sizeof(long long int));
			if(divisoresBloco->divisores == NULL) {
				puts("ERRO - realloc - divisoresBloco->divisores");
				exit(1);
			}
			//secao critica
			pthread_mutex_lock(&lock);
			quantDivisores++;
			pthread_mutex_unlock(&lock);
		}
	}
	pthread_exit(divisoresBloco);
}

//retorna os valores na telas
void printarResultado(long long int quantDivisores, long long int* resultado, long long int numero, double t_fim, double t_ini){
	if(quantDivisores == 2) {
		printf("O numero informado é primo, e seus divisores são: 1 e %lld\n", numero);
	}
	else {
		printf("O numero informado '%lld' é composto e possui %lld divisores:\n", numero, quantDivisores);
		for (long long int i = 0; i < quantDivisores; i++) {
			if(i != quantDivisores - 1){
				printf("%lld, ", resultado[i]);
			} else{
				printf("%lld.\n", resultado[i]);
			}

		}
		// printf("%lld\n", numero);
	}
	printf("Tempo de busca de divisores - Concorrente: %.15lf\n", t_fim - t_ini);
}

//inicialização das threads
void iniciarThread(pthread_t **t_id, int nthreads){
	//alocacao do identificador das threads
	(*t_id) = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
	if(t_id == NULL) {
		puts("ERRO - malloc - t_id");
		exit(1);
	}

	//criacao das threads
	for (long long int i = 0; i < nthreads; i++) {
		if(pthread_create(*t_id+i, NULL, procurarDivisores, (void *) i+1)) {
			printf("Erro ao criar as threads!\n");
			exit(1);
		}
	}
}

retornoSeq* tarefaSequencial(){
	double t_ini_seq, t_fim_seq;
	GET_TIME(t_ini_seq);
	retornoSeq* divisores = (retornoSeq*) malloc(sizeof(retornoSeq)); //struct de retorno de cada thread
	if(divisores == NULL) {
		puts("ERRO - malloc - divisores");
		exit(1);
	}
	divisores->divisoresSeq = (long long int*) malloc(sizeof(long long int)); //divisores que cada thread calculou
	if(divisores->divisoresSeq == NULL) {
		puts("ERRO - malloc - divisores->divisoresSeq");
		exit(1);
	}
	divisores->tamSeq = 0; //numero de divisores calculado por cada thread
	for(long long int i = 1; i <= ((numero)^(1/2)); i++) {
		if(numero % i == 0) {
			divSequencial++;
			divisores->divisoresSeq[divisores->tamSeq] = i;
			divisores->tamSeq++;
			//realoca memoria do vetor de divisores
			divisores->divisoresSeq = (long long int*) realloc(divisores->divisoresSeq, (divisores->tamSeq + 1)*sizeof(long long int));
			if(divisores->divisoresSeq == NULL) {
				puts("ERRO - realloc - divisoresBloco->divisores");
				exit(1);
			}
		}
	}
	GET_TIME(t_fim_seq);
	printf("Tempo Sequencial: %.15lf\n", t_fim_seq-t_ini_seq);
	return divisores;
}

void comparaResposta(long long int quantDivisores, long long int divSequencial, long long int* resultado, retornoSeq* resultadoSeq){
	if(divSequencial == quantDivisores){
		for(long long int i = 0; i<quantDivisores; i++){
			if(resultado[i] != resultadoSeq->divisoresSeq[i]){
				printf("Está diferente!\n");
				break;
			}else{
				printf("Os dois estão iguais.\n");
				break;
			}
		}
	}else{
		printf("Está diferente!\n");
	}
}

//validacao dos parametros de entrada
void validacaoEntrada(long long int numero){

	//tratamento para numeros menores que 1
	if(numero < 1) {
		printf("Escolha numeros naturais maiores que 0\n");
		exit(1);
	}

	//tratamento para 1
	if(numero == 1) {
		printf("Este numero possui apenas ele mesmo como divisor.\n");
		exit(1);
	}
}



void divSort(long long int quantDivisores, long long int* resultado){
	int temp = 0;
	for (int i = 0; i < quantDivisores - 1; i++) {
		for (int j = i+1; j < quantDivisores - 1; j++) {
			if(resultado[i] > resultado[j]) {
				temp = resultado[i];
				resultado[i] = resultado[j];
				resultado[j] = temp;
			}
		}
	}
}


//funcao principal
int main(int argc, char const *argv[]) {
	pthread_t *t_id; //identificadores das threads no sistema
	double t_ini, t_fim; //variaveis para a tomada de tempo
	
	//avaliacao dos parametros de entrada
	if(argc != 3) {
		printf("%s <numero desejado> <numero de threads>\n", argv[0]);
		exit(1);
	}

	nthreads = atoi(argv[2]); //converte para inteiro
	numero = atoll(argv[1]); //converte para long long int
	validacaoEntrada(numero);	

	GET_TIME(t_ini);
	pthread_mutex_init(&lock, NULL);

	iniciarThread(&t_id, nthreads);

	long long int* resultado = (long long int *) malloc(sizeof(long long int)); //todos os divisores
	if(resultado == NULL) {
		puts("ERRO - malloc - resultado");
		exit(1);
	}
	retorno* resultadoParcial; //divisores encontrados ate o momento
	long long int qtdParcial = 0; //quantidade parcial dos divisores
	long long int ini = 0, fim = 0; //posicao inicial e final de cada thread em relacao ao vetor completo de divisores

	for(long long int i = 0; i < nthreads; i++) {
		//espera pelo termino das threads
		if(pthread_join(*(t_id+i), (void **) &resultadoParcial)) {
			printf("Erro nas threads\n");
		}
		fim = resultadoParcial->tam + ini;
		qtdParcial += resultadoParcial->tam;
		resultado = (long long int*) realloc(resultado, (qtdParcial)*sizeof(long long int));
		if(resultado == NULL) {
			puts("ERRO - realloc - resultado");
			exit(1);
		}
		int j = 0;
		//concatena os vetores das threads
		for (long long int i = ini; i < fim; i++) {
			resultado[i] = resultadoParcial->divisores[j];
			j++;
		}
		ini = resultadoParcial->tam + ini;
	}
	GET_TIME(t_fim);

	//ordena os divisores em ordem crescente

	divSort(quantDivisores, resultado);

	printarResultado(quantDivisores, resultado, numero, t_fim, t_ini);

	comparaResposta(quantDivisores, divSequencial, resultado, tarefaSequencial());

	//pega o tempo de liberacao de memoria
	GET_TIME(t_ini);
	pthread_mutex_destroy(&lock);
	free(resultadoParcial);
	free(resultado);
	GET_TIME(t_fim);
	printf("Tempo de liberacao de memoria: %.15lf\n", t_fim - t_ini);

	return 0;
}
