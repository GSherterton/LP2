#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

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
        vec.push_back(it.path().string());
    }

    //por fim retorna o vector com os nomes dos arquivos
    return vec;
}

//esta funcao executa um programa passando como parametro o nome do programa, o argumento 
//que sera uma instancia para ele e o caminho do programa para poder utilizar o exec
void executa(const string& path, const string& programa, const string& instancia){
    cout << "Chamando a instancia " << instancia.c_str() << endl;//apenas um debug para mostrar que esta chamando determinada instancia
    execl((path+"/"+programa).c_str(), programa.c_str(), instancia.c_str(), NULL);
}


//irei criar um processo para cada arquivo de senhas
//irei criar uma thread para cada senha lida no arquivo e tentar descriptografar
//por fim dou um join em cada uma das threads e na ordem lida eu escrevo tudo em um arquivo
//tera um arquivo de senhas descriptografadas para cada arquivo de senhas criptografadas

//esse programa ira apenas criar os forks e chamar a execucao de cada um deles
int main(int argc, char** argv){
    if(argc < 3){
        cout << "Faltando parametros\n";

        return -1;
    }

    string path = current_path();   //recebe o caminho do diretorio atual
    string programa = argv[1];      //o primeiro argumento da execucao é o nome do programa que sera executado
    string pasta = argv[2];         //o segundo argumento da execucao é o nome da pasta que contem as intancias

    vector<string> arquivo = listPath(pasta); //armazena o nome dos arquivos no vector
    
    pid_t originalPid = getpid(); //recebe o PID do processo original para usar de comparacao na hora de dar os forks

    int n = arquivo.size();
    for(int i = 0; i < n; i++){//fazer um fork para cada arquivo que vai ser executado
        if(getpid() == originalPid){//o processo pai é responsavel por dar os forks
            cout << "Dei um fork\n";//apenas um debug para saber quantos forks foram realizados
            fork();
        }
        if(getpid() != originalPid){//os processos filhos irao executar o programa passando o parametro da instancia
            executa(path, programa, arquivo[i]);
        }
    }

    cout << "Encerrando processo pai\n";//apenas um debug para saber se o processo pai conseguiu encerrar com sucesso

    return 0;
}