Para poder executar o programa deve-se primeiro compilar o programa servidor.cpp
Após compilado deve se chamar o executavel de armazem
Mais explicitamente, deve-se usar os comandos:
    g++ servidor.cpp -std=c++17 -lstdc++ -pthread -o servidor
    ./servidor

Depois deve-se abrir outro terminal e enviar os seguintes comandos:
    telnet 127.0.0.1 8080   //para se conectar ao servidor
    DATETIME                //data e hora atual
    RNDNUMBER               //numero aleatorio entre 1 e 100
    UPTIME                  //tempo que o server está ativo
    INFO                    //info do servidor
    BYE                     //encerra a conexão


Com relação a estruturação do trabalho, eu fiz uma pergunta inicial para o usuário escolher o número de robos e caminhoes,
após isso são criadas as threads e inicializadas as barreiras, neste projeto defini 4 barreiras, uma para sincronização
inicial de todas as threads(robos e caminhoes), para iniciarem todas juntas, depois uma barreira para as operações dos robos,
para que todos os robos primeiro finalizem o estoque para depois os caminhoes levarem tudo, uma vez que sem isso, poderia
ser que um caminhao tentasse levar e nao houvesse nenhum item pois ele ainda seria estocado, e outra barreira para sincronizar
o final daquele ciclo de operação que seria referente a todos os caminhoes terem levado itens. A quantidade de iteracoes é 
definida no define QTD_IT e todo o código está comentado, espero que não esteja faltando nenhuma explicação.
Obs: apesar de não estar separado com quebra de linhas a ordem de operações estão todas corretas, no sentido de respeitar o
exemplo fornecido pelo professor