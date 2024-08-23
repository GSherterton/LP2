#define NUM_LETRAS 26
#define ASCII_A 65
#define TAMANHO_SENHA 4
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <pthread.h>

#include <time.h>

using namespace std;

void* funcao(void* p){
    char* num = "BOLA";
    //long num = rand() % 10000;

    //cout << (long)p << " - " << num << endl;

    return ((void*)num);
}

//irei criar um processo para cada arquivo de senhas
//irei criar uma thread para cada senha lida no arquivo e tentar descriptografar
//por fim dou um join em cada uma das threads e na ordem lida eu escrevo tudo em um arquivo
//tera um arquivo de senhas descriptografadas para cada arquivo de senhas criptografadas
int main(int argc, char** argv){
    srand(time(NULL));

    void* num;
    pthread_t thread;
    pthread_create(&thread, NULL, funcao, 0);    
    pthread_join(thread, &num);

    cout << "Num1: " << (char*)num << endl;
    


    // void* num[10];
    // pthread_t thread[10];

    // for(long i = 0; i < 10; i++){
    //     pthread_create(&thread[i], NULL, funcao, (void*)i);    
    // }

    // for(int i = 0; i < 10; i++){
    //     pthread_join(thread[i], &num[i]);
    // }

    // for(int i = 0; i < 10; i++){
    //     cout << "Num1: " << (long)num[i] << endl;
    // }

    return 0;
}