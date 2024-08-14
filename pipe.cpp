#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main(){
    int fd[2]; //0- lado de leitura | 1- lado de escrita

    pipe(fd); //cria o pipe

    pid_t pid = fork();
    

    if(pid == 0){//processo filho
        close(fd[1]);//fecha a escrita

        char msgReceived[100]; //cria um buffer de 99 caracteres
        read(fd[0], msgReceived, sizeof(msgReceived));
        cout << "Mensagem: " << msgReceived << endl;

        close(fd[0]);//fecha a leitura
    }else{//processo pai
        close(fd[0]);//fecha a leitura

        string msg;
        cin >> msg;
        cout << "Passei a msg\n";
        write(fd[1], msg.c_str(), msg.size()+1);

        close(fd[1]);//fecha a escrita
    }

    return 0;
}