#include <unistd.h>
#include <stdlib.h>
#include <iostream> 
#include <string>
#include <vector>
#include <filesystem>
#include <pthread.h>

using namespace std;
using namespace filesystem;

vector<string> listPath(string pasta){
    vector<string> vec;

    for(auto it : directory_iterator(pasta)){
        vec.push_back(it.path().string());
    }

    return vec;
}

struct auxExecuta{
    string path;
    string programa;
    string instancia;

    auxExecuta(){
        path = "";
        programa = "";
        instancia = "";
    }

    auxExecuta(string path, string programa, string instancia){
        this->path = path;
        this->programa = programa;
        this->instancia = instancia;
    }
};

void* executa(void* arg){
    cout << "Executando\n";

    auxExecuta* aux = (auxExecuta*)arg;

    cout << "Chamando a instancia " << aux->instancia.c_str() << endl;
    execl((aux->path+"/"+aux->programa).c_str(), aux->programa.c_str(), aux->instancia.c_str(), NULL);

    return NULL;
}


//aparentemente n faz sentido usar thread para fazer isso,
//se eu trocar o contexto da thread acredito que ela morre
int main(int argc, char** argv){   
    if(argc < 3){
        cout << "Missing parameters\n";

        return -1;
    }

    string path = current_path(); //recebe o caminho do diretorio atual
    string programa = argv[1];
    string pasta = argv[2];

    vector<string> arquivo = listPath(pasta);

    int n = arquivo.size();

    pthread_t thread[n];

    vector<auxExecuta*> aux(n, new auxExecuta(path, programa, ""));
    
    /*aux->path = path;
    aux->programa = programa;*/

    for(int i = 0; i < n; i++){//cria uma thread que vai executar o programa com cada instancia
        aux[i]->instancia = arquivo[i];
        pthread_create(&thread[i], NULL, executa, (void*)aux[i]);
    }

    for(int i = 0; i < n; i++){
        pthread_join(thread[i], 0);
    }

    cout << "Encerrando\n";

    return 0;
}