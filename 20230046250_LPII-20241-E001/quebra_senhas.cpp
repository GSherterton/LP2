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
#include <filesystem>
#include <pthread.h>

using namespace std;
using namespace filesystem;

//funcao que criptografa a senha, fornecida pelo professor
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

//funcao que armazena as senhas no vector
vector<string> read(const string& arquivo){
    //cria um fluxo de entrada (para ler o arquivo)
    ifstream fp;

    //tenta abrir o arquivo
    fp.open(arquivo);

    //verifica se o arquivo foi aberto
    if(!fp.is_open()){
        cout << "Nao foi possivel abrir o arquivo\n";
        return vector<string>();
    }

    //crio o vector que sera retornado e dou um reserve de 10 arbitrariamente
    //apenas pois sei que tem 10 senhas em cada arquivo 
    vector<string> vec;
    vec.reserve(10);

    //crio uma string auxiliar para poder dar push back no vector
    string senha;

    //leio todas as linhas do arquivo e armazeno cada uma dela no vector
    while(1){
        getline(fp, senha);

        if(fp.eof()){
            break;
        }

        vec.push_back(senha);
    }

    fp.close();

    return vec;
}

//verifica se a quantidade de parametros passados na chamada do programa esta correta
bool argcIsValid(const int& argc){
    if(argc < 2){
        cout << "Esta faltando o nome do arquivo.\n";
        return false;
    }else if(argc > 3){
        cout << "Mais parametros do que o esperado.\n";
        return false;
    }

    return true;
}

//irei criar um processo para cada arquivo de senhas
//irei criar uma thread para cada senha lida no arquivo e tentar descriptografar
//por fim dou um join em cada uma das threads e na ordem lida eu escrevo tudo em um arquivo
//tera um arquivo de senhas descriptografadas para cada arquivo de senhas criptografadas
int main(int argc, char** argv){
    //verifica se a quantidade de parametros passados na chamada do programa esta correta
    if(!argcIsValid(argc)){
        return -1;
    }

    string arquivo = argv[1];

    //cria um vector de senhas com as senhas do arquivo
    vector<string> senhasCriptografadas = read(arquivo);
    int n = senhasCriptografadas.size();//armazena a qtd de senhas

    //cria um vector para guardar as senhas descriptografadas
    vector<string> senhasDescriptografadas;
    senhasDescriptografadas.resize(n);

    /*
    //criar as variaveis do tipo thread para cada senha presente no arquivo
    pthread_t thread[n];

    //cria uma thread que vai executar a funcao com cada senha
    for(int i = 0; i < n; i++){
        pthread_create(&thread[i], NULL, executa, (void*)aux[i]);
    }

    //da um join em cada thread
    for(int i = 0; i < n; i++){
        pthread_join(thread[i], senhasDescriptografadas[i]);
    }
    */


    for(int i = 0; i < senhasCriptografadas.size(); i++){
        cout << "Senha [" << i << "]: " << senhasCriptografadas[i] << endl;
    }

    cout << "Rodando " << arquivo << endl;

    return 0;
}