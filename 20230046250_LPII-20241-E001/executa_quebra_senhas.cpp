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
#include <time.h>

using namespace std;
using namespace filesystem;

//esta funcao le o nome de uma pasta e retorna um vector
//contendo os nomes de cada arquivo dentro dela
vector<string> listPath(const string& pasta){
    //cria o vector que sera retornado contendo os nomes dos arquivos
    vector<string> vec;

    //faz um contador para saber qual o tamanho da pasta e assim poder dar
    //um reserve na memoria do vector para otimizar a insercao de elementos
    int cont = 0;
    for(auto it : directory_iterator(pasta)){
        cont++;
    }

    vec.reserve(cont);

    //itera sobre a pasta e da um push back em cada nome de arquivo na pasta
    for(auto it : directory_iterator(pasta)){
        vec.push_back(it.path().filename().string());
    }

    //por fim retorna o vector com os nomes dos arquivos
    return vec;
}

struct auxExecucao{
    string path;
    string pasta;
    string arquivo;
    string saida;

    auxExecucao();
    auxExecucao(string path, string pasta, string saida){
        this->path = path;
        this->pasta = pasta;
        this->saida = saida;
    }
};

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

char* descriptografa(void* senha){
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
            return result;
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

void write(vector<string> senhas, const int& n, const string& saida){
    //cria um fluxo de saida (para escrever no arquivo)
    ofstream fp;

    //tenta abrir o arquivo
    fp.open(saida);

    //verifica se o arquivo foi aberto
    if(!fp.is_open()){
        cout << "Nao foi possivel abrir o arquivo\n";
        return;
    }

    //escreve todas as senhas no arquivo
    for(int i = 0; i < n; i++){
        fp << senhas[i] << endl;
    }

    //fecha o arquivo
    fp.close();
}

//esta funcao executa a descriptografia das senhas do arquivo, cujo caminho é passado 
//como parametro e direciona elas para uma saida também passada como parametro
void* executa(void* p){
    auxExecucao* aux = (auxExecucao*)p;

    //le o nome do argumento que representa a pasta e o nome do arquivo
    string caminho = aux->pasta + aux->arquivo;

    //le o nome do argumento que representa o nome da pasta de saida
    string saida = aux->saida+"dec_"+aux->arquivo;

    //cria um vector de senhas com as senhas do arquivo
    vector<string> senhasCriptografadas = read(caminho);

    int n = senhasCriptografadas.size();//armazena a qtd de senhas

    //cria um array para guardar as senhas descriptografadas
    vector<string> senhasDescriptografadas(n);

    //executa a funcao com cada senha
    for(int i = 0; i < n; i++){
        senhasDescriptografadas[i] = descriptografa((void*)senhasCriptografadas[i].c_str());
    }

    //escreve as senhas descriptografadas em uma pasta especificada
    write(senhasDescriptografadas, n, saida);

    return NULL;
}

//esse programa concede a escolha do usuario de utilizar threads ou processos, e baseado
//nessa escolha cria 10 estruturas(de acordo com a escolhida, e 10 por terem 10 arquivos)
int main(int argc, char** argv){
    if(argc < 3){
        cout << "Faltando parametros\n";

        return -1;
    }

    int escolha = 0;
    while(1){//le a escolha e garante que seja uma opcao valida
        if(escolha != 1 && escolha != 2){
            cout << "Voce deseja utilizar threads ou processos para quebrar as senhas:\n";
            cout << "1- Processos | 2- Threads\n";
            cin >> escolha;
        }else{
            break;
        }
    }

    //contagem de tempo
    clock_t start, end;
    double tempo = 0;

    start = clock(); //inicia o contador de tempo

    string path = current_path();   //recebe o caminho do diretorio atual
    string pasta = argv[1];         //o primeiro argumento da execucao é o nome da pasta que contem as intancias
    string saida = argv[2];         //o segundo argumento da execucao é o nome da pasta que ira conter as saidas

    vector<string> arquivo = listPath(pasta); //armazena o nome dos arquivos no vector

    int n = arquivo.size();

    if(escolha == 1){//Escolheu Processos
        cout << "Gerando 10 processos para processar arquivos...\n";

        //cria uma struct auxiliar para passar como parametro na função
        auxExecucao* aux = new auxExecucao(path, pasta, saida);

        pid_t originalPid = getpid(); //recebe o PID do processo original para usar de comparacao na hora de dar os forks
        vector<int> PIDs(n); //cria um vector para armazenar os pids dos processos gerados
        for(int i = 0; i < n; i++){//fazer um fork para cada arquivo que vai ser executado
            if(getpid() == originalPid){//o processo pai é responsavel por dar os forks
                PIDs[i] = fork();
            }
            if(getpid() != originalPid){//os processos filhos irao executar a descriptografia passando o parametro do arquivo
                aux->arquivo = arquivo[i];

                cout << "Processo PID " << getpid() << ": Quebrando senhas de " << arquivo[i].c_str() << endl;//mostra que esta executando tal arquivo

                executa((void*)aux);
                //executa(path, programa, pasta, arquivo[i], saida);

                return 0;
            }
        }

        if(aux != NULL){
            delete aux;
        }
        
        for(int i = 0; i < n; i++){
            waitpid(PIDs[i], 0, 0);

            cout << "Processo PID " << getpid() << ": Senhas quebradas salvas em " << saida+"dec_"+arquivo[i] << endl;
        }
    }else if(escolha == 2){//Escolheu Threads
        vector<auxExecucao*> aux(n);
        for(int i = 0; i < n; i++){
            aux[i] = new auxExecucao(path, pasta, saida);
        }

        cout << "Criando 10 threads para processar arquivos...\n";

        //criar as variaveis do tipo thread para cada arquivo
        pthread_t thread[n];

        //cria uma thread que vai executar a funcao com cada arquivo
        for(int i = 0; i < n; i++){
            aux[i]->arquivo = arquivo[i];

            cout << "Thread #" << i << ": Quebrando senhas de " << arquivo[i].c_str() << endl;//mostra que esta executando tal arquivo

            pthread_create(&thread[i], NULL, executa, (void*)aux[i]);
        }

        //da um join em cada thread
        for(int i = 0; i < n; i++){
            pthread_join(thread[i], 0);
            cout << "Thread #" << i << ": Senhas quebradas salvas em " << saida+"dec_"+arquivo[i] << endl;
        }

        for(int i = 0; i < n; i++){
            if(aux[i] != NULL){
                delete aux[i];
            }
        }
    }else{
        cout << "Erro!\n";
        return -1;
    }

    
    end = clock(); //pega o tempo final
    tempo = (double)(end - start) / (double)(CLOCKS_PER_SEC); //realiza o cálculo do tempo gasto pelo algoritmo

    cout << "Tempo de execução: " << tempo << "s" << endl;

    return 0;
}