#include <iostream>
#include <pthread.h>

#define QTD_IT 3

using namespace std;

int num_itens = 0;

//barreiras que simulam o inicio e o fim de cada ciclo de operação,
//alem das partes dos robos e dos caminhoes especificamente
pthread_barrier_t barreira_inicio;
pthread_barrier_t barreira_robo;
pthread_barrier_t barreira_caminhao;
pthread_barrier_t barreira_fim;

//definindo as travas utilizadas
pthread_mutex_t trava_robo = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t trava_caminhao = PTHREAD_MUTEX_INITIALIZER;

void* robo_function(void* p){
    long idx = (long)p;

    int i = 0;
    while(i < QTD_IT){
        printf("Robô de inspeção %ld está pronto para inspecionar.\n", idx+1);
        pthread_barrier_wait(&barreira_inicio); //sincroniza o inicio de todas as threads

        pthread_mutex_lock(&trava_robo); // Trava os robos
        num_itens++;
        printf("Robô de inspeção %ld inspecionou e estocou um item. Itens no estoque: %d\n", idx+1, num_itens);
        pthread_mutex_unlock(&trava_robo); // Destrava os robos

        //indica que todos os robos ja fizeram suas operacoes e agora os caminhoes podem prosseguir
        pthread_barrier_wait(&barreira_robo);
        pthread_barrier_wait(&barreira_caminhao);

        //Espera todas as threads finalizarem para poder prosseguir novamente
        pthread_barrier_wait(&barreira_fim);

        i++;
    }

    return NULL;
}

void* caminhao_function(void* p){
    long idx = (long)p;

    int i = 0;
    while(i < QTD_IT){
        printf("Caminhão %ld está pronto para carregar.\n", idx+1);
        pthread_barrier_wait(&barreira_inicio); //sincroniza o inicio de todas as threads

        pthread_barrier_wait(&barreira_caminhao);//espera todos os robos finalizarem suas operações

        pthread_mutex_lock(&trava_caminhao); // Trava os caminhoes
        if(num_itens){//se tem algum item
            num_itens--;
            printf("Caminhão %ld carregou um item. Itens restantes no estoque: %d\n", idx+1, num_itens);    
        }else{
            printf("Caminhão %ld ocioso pois não há nenhum item no estoque!\n", idx+1);    
        }
        pthread_mutex_unlock(&trava_caminhao); // Destrava os caminhoes

        //Espera todas as threads finalizarem para poder prosseguir novamente
        pthread_barrier_wait(&barreira_fim);

        i++;
    }

    return NULL;
}

int main(){
    int num_robos = 0;
    int num_caminhoes =  0;

    while(1){//le os valores de forma correta
        cout << "Digite o numero de robos: ";
        cin >> num_robos;

        cout << "Digite o numero de caminhoes: ";
        cin >> num_caminhoes;

        if(num_robos && num_caminhoes){ //se são valores válidos
            break;
        }
    }
    
    //criar as variaveis do tipo thread para cada robo e caminhao
    pthread_t robos[num_robos];
    pthread_t caminhoes[num_caminhoes];

    //inicializa a barreira para a quantidade de robos e caminhoes
    pthread_barrier_init(&barreira_inicio, NULL, num_robos+num_caminhoes);
    pthread_barrier_init(&barreira_robo, NULL, num_robos);
    pthread_barrier_init(&barreira_caminhao, NULL, num_robos+num_caminhoes);
    pthread_barrier_init(&barreira_fim, NULL, num_robos+num_caminhoes);

    //cria uma thread para cada robo e caminhao
    for(long i = 0; i < num_robos; i++){
        pthread_create(&robos[i], NULL, robo_function, (void*)i);
    }
    for(long i = 0; i < num_caminhoes; i++){
        pthread_create(&caminhoes[i], NULL, caminhao_function, (void*)i);
    }

    //da um join em cada thread
    for(int i = 0; i < num_robos; i++){
        pthread_join(robos[i], NULL);
    }
    for(int i = 0; i < num_caminhoes; i++){
        pthread_join(caminhoes[i], NULL);
    }

    printf("Todos os robôs de inspeção e caminhões completaram suas operações.\n");

    return 0;
}