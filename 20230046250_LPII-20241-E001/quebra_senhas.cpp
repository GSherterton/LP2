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
#include <string.h>

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

//funcao que vai incrementar a string que sera testada
void incrementaString(char* str, const int& index){
    //condicao de parada caso de "overflow" na string
    if(index < 0){
        return;
    }

    if(str[index] == 'Z'){
        str[index] = 'A';
        incrementaString(str, index-1);
    }else{
        str[index]++;
    }
}

bool iguais(const char* str1, const char* str2, const int& qtd){
    int i = 0;
    while(i < qtd){
        if(str1[i] == str2[i]){
            i++;
        }else{
            return false;
        }
    }

    return true;
}

void* descriptografa(void* senha){
    //inicializa os testes com AAAA
    char teste[] = "AAAA";

    while(1){
        //se bateu a senha que eu queria achar com o resultado do meu teste
        //entao achei a senha descriptografada
        if(iguais(encrypt(teste), (char*)senha, 4)){
            //teste eh um array nao um ponteiro, e eh local, logo eh necessario criar
            //um ponteiro para char que armazene a mesma coisa e seja usado no retorno
            char* result = (char*) new char(strlen(teste)+1);
            strcpy(result, teste);
            return (void*)result;
        }

        //finaliza caso testar com ZZZZ e nao achar
        if(strcmp(teste, "ZZZZ") == 0){
            break;
        }

        incrementaString(teste, strlen(teste)-1);
    }

    //Nao encontrou a senha
    cout << "Nao encontrei a senha\n";
    return NULL;
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

    //fecho o arquivo
    fp.close();

    //retorno o vector
    return vec;
}

//verifica se a quantidade de parametros passados na chamada do programa esta correta
bool argcIsValid(const int& argc){
    if(argc < 2){
        cout << "Esta faltando o nome do arquivo.\n";
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
    
    //le o nome do argumento que representa o nome do arquivo da instancia
    string arquivo = argv[1];

    //cria um vector de senhas com as senhas do arquivo
    vector<string> senhasCriptografadas = read(arquivo);

    int n = senhasCriptografadas.size();//armazena a qtd de senhas

    //cria um array para guardar as senhas descriptografadas
    void* senhasDescriptografadas[n];

    //criar as variaveis do tipo thread para cada senha presente no arquivo
    pthread_t thread[n];

    //cria uma thread que vai executar a funcao com cada senha
    for(int i = 0; i < n; i++){
        pthread_create(&thread[i], NULL, descriptografa, (void*)(senhasCriptografadas[i].c_str()));
    }

    //da um join em cada thread e armazena a resposta nesse array
    for(int i = 0; i < n; i++){
        pthread_join(thread[i], &senhasDescriptografadas[i]);
    }

    //imprimi cada senha descriptografada em ordem
    for(int i = 0; i < n; i++){
        cout << (char*)senhasDescriptografadas[i] << endl;
    }

    return 0;
}