#include <string>
#include <vector>
#include <iostream>
#include <thread>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

using namespace std;

//vector de conexoes, indicando se a conexao esta sendo feita e uma variavel para a conexao do accept
vector<pair<bool, int>> conexoes;

//ver se não precisa desses vector
vector<struct sockaddr_in> connection_address;
vector<int> connection_addrlen;


//erro está dando por passar essa referencia, melhor passar o vector msm,
//pois quando o vector muda de tamanho a referencia vai embora
void handleClient(int i) {
	// bool* is_connected = &conexoes[i].first;
    // int* fd = &conexoes[i].second;

    // printf("My fd is: %d\n", *fd);
    // printf("My fd is: %d\n", conexoes[i].second);

    // printf("[Thread] %s\n", ((is_connected)?("Conectado"):("Disconectado"))); //debug

	// char cli_buf[CLI_BUF_SIZE];
	// char* welcome_msg = "Hello, Client!\n";

	// send(fd, welcome_msg, strlen(welcome_msg), 0);

	// printf("Enviei %s para o novo cliente\n", welcome_msg);

	// recv(fd, cli_buf, CLI_BUF_SIZE, 0);

	// printf("Recebi do cliente (%d): %s\n", fd, cli_buf);

    sleep(5);

    // printf("Desconectando\n");

    conexoes[i].first = 0;

    // printf("Fechando fd[%d]\n", conexoes[i].second);

	close(conexoes[i].second);

    // printf("[Thread] %s\n", ((is_connected)?("Conectado"):("Disconectado"))); //debug
}


int main(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    // char *message = "Hello, Client!\n";
    string message = "Hello, Client!\n";

    // Cria o socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket falhou");
        exit(EXIT_FAILURE);
    }

    // Configura o endereço e a porta
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Associa o socket ao endereço e porta
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind falhou");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Coloca o socket em modo de escuta
    if (listen(server_fd, 10) < 0) {
        perror("listen falhou");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor esperando conexões na porta %d\n", PORT);

    //vector de threads
    vector<thread> threads;

    while(1){
        int i;
        printf("Conexoes size: %ld\n", conexoes.size());
        for(i = 0; i < conexoes.size(); i++){
            //verifica se não ha alguma conexao ja sendo feita
            if(!conexoes[i].first){
                printf("Achei uma conexao livre!\n");
                //achou uma conexao livre
                break;
            }
        }

        //se nao ha thread disponivel
        if(i == conexoes.size()){
            connection_address.push_back(address);
            connection_addrlen.push_back(addrlen);
            conexoes.push_back(make_pair(0, 0));
            printf("Criei nova conexao!\n");
        }

        // Aceita uma conexão
        if((conexoes[i].second = accept(server_fd, (struct sockaddr *)&connection_address[i],
            (socklen_t*)&connection_addrlen[i])) < 0) {
            perror("accept falhou");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Nova conexao feita com sucesso! [%d]\n", conexoes[i].second);
        printf("No idx [%d]\n", i);

        conexoes[i].first = 1;

        // printf("[0] %s\n", ((conexoes[i].first)?("Conectado"):("Disconectado")));

        if(i == threads.size()){
            // threads.push_back(thread(handleClient, (void*)conexoes[i].first, (void*)(long)conexoes[i].second));
            threads.push_back(thread(handleClient, i));
        }else{
            // threads[i] = thread(handleClient, (void*)conexoes[i].first, (void*)(long)conexoes[i].second);
            printf("Entrei aqui\n");
            threads[i].join();
            threads[i] = thread(handleClient, i);
        }

        // sleep(1);
        // printf("[1] %s\n", ((conexoes[i].first)?("Conectado"):("Disconectado")));
    }


    // // Aceita uma conexão
    // if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
    //     (socklen_t*)&addrlen)) < 0) {
    //     perror("accept falhou");
    //     close(server_fd);
    //     exit(EXIT_FAILURE);
    // }

    // // send(new_socket, message, strlen(message), 0);
    // send(new_socket, message.c_str(), message.size(), 0);
    // printf("Mensagem enviada ao cliente.\n");

    // close(new_socket);
    close(server_fd);

    return 0;
}

/*
#include <iostream>
#include <pthread.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BACKLOG_SIZE 5
#define CLI_BUF_SIZE 100
#define QTD_CLI 5

int cli_fd[QTD_CLI];

using namespace std;

void* handle_client(void* p) {
	int fd = (int)(long)p;

	char cli_buf[CLI_BUF_SIZE];
	char* welcome_msg = "Hello, Client!\n";

	send(fd, welcome_msg, strlen(welcome_msg), 0);

	printf("Enviei %s para o novo cliente\n", welcome_msg);

	recv(fd, cli_buf, CLI_BUF_SIZE, 0);

	printf("Recebi do cliente (%d): %s\n", fd, cli_buf);

	close(fd);

}

int main(){
    int serv_fd;
	struct sockaddr_in address;
	struct sockaddr_in cli_addr[QTD_CLI];
	int cli_addr_size[QTD_CLI];
	pthread_t cli_threads[QTD_CLI];

	int cli_count = 0;

	if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind()");
		exit(EXIT_FAILURE);
	}

	if (listen(serv_fd, BACKLOG_SIZE) < 0) {
		perror("listen()");
		exit(EXIT_FAILURE);
	}

	printf("Servidor aguardando conexoes na porta %d\n", PORT);

	while(1) {
		struct sockaddr_in cli_addr_temp;
		int cli_addr_c_temp = 0;
		if ((cli_fd[cli_count] = accept(serv_fd, (struct sockaddr*)&cli_addr_temp, &cli_addr_c_temp)) < 0) {
			perror("accept()");
			exit(EXIT_FAILURE);
		}
		pthread_create(&cli_threads[cli_count], 0, handle_client, (void*)(long)cli_fd[cli_count]);
		printf("Recebi nova conexao: %d\n", ++cli_count);
	}

//	send(cli_fd, welcome_msg, strlen(welcome_msg), 0);

//	recv(cli_fd, cli_buf, CLI_BUF_SIZE, 0);

//	printf("Recebi do cliente: %s\n", cli_buf);

//	close(cli_fd);

    return 0;
}
*/