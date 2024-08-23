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

void* descriptografa(void* senha){
    //inicializa os testes com AAAA
    char teste[] = "AAAA";

    //cout << "Senha: " << (char*) senha << endl;//debug

    while(1){
        // cout << "Str: " << teste << endl;//debug

        //se bateu a senha que eu queria achar com o resultado do meu teste
        //entao achei a senha descriptografada
        if(strcmp(encrypt(teste), (char*)senha) == 0){
            cout << "Senha encontrada: " << teste << endl;//debug
            //teste eh um array nao um ponteiro, e eh local, logo eh necessario criar
            //um ponteiro para char que armazene a mesma coisa e seja usado no retorno
            char* result = (char*) new char(strlen(teste)+1);
            strcpy(result, teste);
            return (void*)result;
        }

        //finaliza caso testar com ZZZZ e nao achar
        if(strcmp(teste, "ZZZZ") == 0){
            cout << "Cheguei em ZZZZ\n";
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

    //tentando rodar uma vez so

    //cria um array para guardar as senhas descriptografadas
    void* senhasDescriptografadas;

    //criar as variaveis do tipo thread para cada senha presente no arquivo
    pthread_t thread;

    /*
    //cria uma thread que vai executar a funcao com cada senha
    pthread_create(&thread, NULL, descriptografa, (void*)(senhasCriptografadas[0].c_str()));

    //da um join em cada thread e armazena a resposta nesse array
    pthread_join(thread, &senhasDescriptografadas);
    */

    //visualiza as senhas criptografadas
    cout << "Senhas Criptografadas:\n";
    cout << "Senha [0]: " << senhasCriptografadas[0] << endl;
    
    cout << endl;


    void* ans = descriptografa((void*)(senhasCriptografadas[0].c_str()));

    //visualiza as senhas descriptografadas
    cout << "Senha Descriptografada:\n";
    cout << "Senha [0]: " << (char*)ans << endl;

    cout << "Rodando " << arquivo << endl;


    // int n = senhasCriptografadas.size();//armazena a qtd de senhas

    // //cria um array para guardar as senhas descriptografadas
    // void* senhasDescriptografadas[n];

    // //criar as variaveis do tipo thread para cada senha presente no arquivo
    // pthread_t thread[n];

    // //cria uma thread que vai executar a funcao com cada senha
    // for(int i = 0; i < n; i++){
    //     pthread_create(&thread[i], NULL, descriptografa, (void*)(senhasCriptografadas[i].c_str()));
    // }

    // //da um join em cada thread e armazena a resposta nesse array
    // for(int i = 0; i < n; i++){
    //     pthread_join(thread[i], &senhasDescriptografadas[i]);
    // }

    // //visualiza as senhas criptografadas
    // cout << "Senhas Criptografadas:\n";
    // for(int i = 0; i < n; i++){
    //     cout << "Senha [" << i << "]: " << senhasCriptografadas[i] << endl;
    // }

    // cout << endl;

    // //visualiza as senhas descriptografadas
    // // for(int i = 0; i < senhasCriptografadas.size(); i++){
    // cout << "Senhas Descriptografadas:\n";
    // for(int i = 0; i < n; i++){
    //     cout << "Senha [" << i << "]: " << (char*)senhasDescriptografadas[i] << endl;
    // }

    // cout << "Rodando " << arquivo << endl;
    //-------------------------------------------------------------------------------------------
    /*
    char senha[] = "LCII";
    cout << encrypt(senha) << endl;

    string str;
    cin >> str;

    void* ans; // = descriptografa((void*)(str.c_str()));

    pthread_t thread;
    pthread_create(&thread, NULL, descriptografa, (void*)(str.c_str()));
    pthread_join(thread, &ans);

    cout << "Resposta: " << (char*)ans << endl;
    */

    return 0;
}