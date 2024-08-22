#define NUM_LETRAS 26
#define ASCII_A 65
#define TAMANHO_SENHA 4
#include <stdio.h>

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
}

int main(){
    


    return 0;
}