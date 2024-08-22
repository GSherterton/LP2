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

vector<string> listPath(const string& pasta){
    vector<string> vec;
    //vec.resize(pasta.size());

    for(auto it : directory_iterator(pasta)){
        vec.push_back(it.path().string());
    }

    return vec;
}

void executa(const string& path, const string& programa, const string& instancia){
    cout << "Chamando a instancia " << instancia.c_str() << endl;
    execl((path+"/"+programa).c_str(), programa.c_str(), instancia.c_str(), NULL);
}

int main(int argc, char** argv){
    if(argc < 3){
        cout << "Missing parameters\n";

        return -1;
    }

    string path = current_path(); //recebe o caminho do diretorio atual
    string programa = argv[1];
    string pasta = argv[2];

    vector<string> arquivo = listPath(pasta);
    
    pid_t originalPid = getpid();

    int n = arquivo.size();
    for(int i = 0; i < n; i++){//fazer um fork para cada arquivo que vai ser executado
        if(getpid() == originalPid){//o processo pai
            cout << "Dei um fork\n";
            fork();
        }
        if(getpid() != originalPid){//faz o processo filho executar o programa passando o parametro da instancia
            executa(path, programa, arquivo[i]);
        }
    }

    cout << "Encerrando processo pai\n";

    return 0;
}