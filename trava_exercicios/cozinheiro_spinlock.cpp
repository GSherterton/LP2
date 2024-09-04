#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <time.h>
#include <atomic>

#define QTD_PRATOS 8

using namespace std;

struct Prato{
    string descricao;
    int tempo;
};

//lista dos patros com seus nomes e tempo de execucao
Prato pratos[QTD_PRATOS] = {{"Ovo frito", 1},
                            {"File a parmegiana", 4},
                            {"Feijoada", 6},
                            {"Sushi", 3},
                            {"Misto Quente", 1},
                            {"Macarronada", 3},
                            {"Suco de Laranja", 2},
                            {"Milkshake", 2}};

//spinlock
atomic_flag lock = ATOMIC_FLAG_INIT;

void acquire_lock() {
    while(atomic_flag_test_and_set(&lock));// Espera ocupada
}

void release_lock() {
    atomic_flag_clear(&lock);
}                            

//funcao que simula a preparacao de um prato
void* prepara_prato(void* p){
    long cozinheiro = (long)p;
    int index = rand() % QTD_PRATOS;

    acquire_lock();
    cout << "Cozinheiro [#" << cozinheiro << "] preparando um(a) " << pratos[index].descricao << endl;
    sleep(pratos[index].tempo);
    release_lock();

    return NULL;
}

int main(){
    //inicia a seed de geracao do rand
    srand(time(NULL));

    int n;
    cout << "Digite a quantidade de cozinheiros: ";
    cin >> n;

    //cria as n threads representando cada cozinheiro e faz eles prepararem um prato aleatorio
    pthread_t cozinheiros[n];

    for(long i = 0; i < n; i++){
        pthread_create(&cozinheiros[i], NULL, prepara_prato, (void*)i);
    }

    for(long i = 0; i < n; i++){
        pthread_join(cozinheiros[i], 0);
    }

    return 0;
}