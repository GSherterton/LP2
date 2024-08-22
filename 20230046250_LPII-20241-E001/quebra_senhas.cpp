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
#include <filesystem>
#include <pthread.h>

using namespace std;
//using namespace filesystem;

//esta funcao le o nome de uma pasta e retorna um vector
//contendo os nomes de cada arquivo dentro dela
/*vector<string> listPath(string pasta){
    vector<string> vec;

    for(auto it : filesystem::directory_iterator(pasta)){
        vec.push_back(it.path().string());
    }

    return vec;
}

//esta funcao executa um programa passando como parametro o nome do programa e 
void executa(string path, string programa, string instancia){
    cout << "Chamando a instancia " << instancia.c_str() << endl;
    execl((path+"/"+programa).c_str(), programa.c_str(), instancia.c_str(), NULL);
}

char* encrypt(const char* str) {
    char* str_result = (char*) malloc(sizeof(char) * (TAMANHO_SENHA + 1));

    if(!str_result) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < TAMANHO_SENHA; i++) {
        char c = str[i];
        char chave = str[i];

        if(c >= 'A' && c <= 'Z') {
            int str_idx = c - ASCII_A;
            int chave_idx = chave - ASCII_A;
            str_result[i] = ((str_idx + chave_idx) % NUM_LETRAS) + ASCII_A;
        }else{
            perror("Erro: String contém caracteres inválidos.");
            free(str_result);
            exit(EXIT_FAILURE);
        }
    }
    
    str_result[TAMANHO_SENHA] = '\0'; // Finaliza a string
    return str_result;
}*/

//irei criar um processo para cada arquivo de senhas
//irei criar uma thread para cada senha lida no arquivo e tentar descriptografar
//por fim dou um join em cada uma das threads e na ordem lida eu escrevo tudo em um arquivo
//tera um arquivo de senhas descriptografadas para cada arquivo de senhas criptografadas
int main(){
    vector<string> vec;
    vec.reserve(5);
    for(int i = 0; i < 6; i++){
        vec.push_back(to_string(i));
    }
    cout << vec.capacity() << endl;

    return 0;
}