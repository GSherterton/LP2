#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

//exemplo de como capturar o retorno de um fork com waitpid
//entretanto so consegue capturar um valor de 0 a 255
int main(){
    pid_t pid = fork();

    if(pid == 0){//processo filho
        exit();
    }else{//processo pai
        int retorno;
        waitpid(pid, &retorno, 0);
        cout << WEXITSTATUS(retorno) << endl;
    }

    return 0;
}