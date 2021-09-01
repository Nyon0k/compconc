#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int num;
pthread_mutex_t mutex;
pthread_cond_t cond;
int *arr;
int blockeds = 0;

void wall(int num, long id) {
    pthread_mutex_lock(&mutex);
    if(blockeds == (num-1)){ 
        printf("        T%ld: Desbloqueou Wall\n\n", id);
        pthread_cond_broadcast(&cond);
        blockeds = 0;
    }else{
        printf("        T%ld: Wall\n", id);
        blockeds++;
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void *calculadora(void *args){
    long id = (long) args;
    int *sumParcial = malloc(sizeof(int));

    if(sumParcial==NULL){
        fprintf(stderr, "ERRO--malloc\n");
        exit(2);
    }

    for(int i=1; i<=num; i++){
        for(int j=1; j<=num; j++){
            *sumParcial += arr[j-1];
        }

        printf("    Passo 1:\n");
        printf("        T%ld: Sum = %d\n", id, *sumParcial);
        wall(num, id);

        printf("    Passo 2:\n");
        arr[id] = rand()%10;
        printf("        T%ld: Index %ld do array recebe %d\n", id, id, arr[id]);
        wall(num, id);
    }
    printf("        T%ld: Fim\n", id);
    pthread_exit(sumParcial);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t tids[num];
    int *sumTotal[num];
    arr = malloc(sizeof(int) *num);

    if(argc < 2){
        fprintf(stderr, "Digite %s <num>\n", argv[0]);
        exit(1);
    }
    num = atoll(argv[1]);

    if(arr == NULL){
        fprintf(stderr, "ERRO--malloc\n");
        exit(2);
    }
    for(int i=0; i<num; i++){
        arr[i] = rand()%10;
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Printa array original
    printf("\nArray inicial:\n");
    for(int i=0; i<num; i++){
        printf("    %d: %d\n", i, arr[i]);
    }
    printf("    Legenda -> Index: Valor\n\n");

    printf("Começo:\n\n");
    for(long i=0; i<num; i++){
        if(pthread_create(&tids[i], NULL, calculadora, (void *) i)) {
            printf("ERRO--pthread_create\n");
            exit(3);
        }
    }

    for(int i=0; i<num; i++){
        pthread_join(tids[i], (void **) &sumTotal[i]);
    }

    printf("\n    Legenda -> Uma iteração ocorre no término de um 'Passo 2'.\n      -> Um passo termina quando tem espaço entre as linhas.\n\n");

    printf("Fim:\n\n");
    
    printf("    Array final:\n");
    for(int i=0; i<num; i++){
        printf("        %d: %d\n", i, arr[i]);
    }
    printf("        Legenda -> Index: Valor\n\n");

    printf("    Somatórios finais: \n");
    for(int i=0; i<num; i++){
        printf("        Thread %d = %d\n", i, *(sumTotal[i]));
    }
    printf("\n");

    int flag = 0;
    for(int i=1; i<num; i++){
        if(*(sumTotal[i]) != *(sumTotal[0])){
            flag = 0;
        }
        flag = 1;
    }
    if(flag == 0){
        printf("        Somatórios diferentes!\n\n");
    }else{
        printf("        Somatórios iguais!\n\n");
    }

    // Libera memória
    free(arr);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    for(int i=0; i<num; i++){
        free(sumTotal[i]);
    }

    return 0;
}