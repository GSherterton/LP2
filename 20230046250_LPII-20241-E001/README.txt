Para poder executar o programa deve-se primeiro compilar os programas quebra_senhas.cpp e o executa_quebra_senhas.cpp
Após compilados deve se chamar o executavel de executa_quebra_senhas passando como parametros o nome do executavel de
quebra_senhas, o nome da pasta contendo as senhas e o nome da pasta onde ficarão os resultados
Mais explicitamente, deve-se usar os comandos:
    g++ quebra_senhas.cpp -o quebra_senhas
    g++ executa_quebra_senhas.cpp -o executa
    ./executa quebra_senhas ./senhas/ ./decodificadas/


Com relação a estruturação do trabalho, eu fiz uma ideia de ler os arquivos que estao na pasta das senhas e realizar um
fork para cada arquivo, para realizar o paralelismo também dentro de cada processo eu utilizei as threads para gerar uma
para cada senha contida no arquivo, ou seja, neste caso terão 11 processos rodando, o pai que chama os forks e os filhos,
e dentro de cada processo filho terão 10 threads, logo 100 threads serão criadas. Todo o código está comentado, espero que
não esteja faltando nenhuma explicação