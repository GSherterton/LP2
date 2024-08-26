Para poder executar o programa deve-se primeiro compilar o programa executa_quebra_senhas.cpp
Após compilado deve se chamar o executavel de executa_quebra_senhas passando como parametros
o nome da pasta contendo as senhas e o nome da pasta onde ficarão os resultados
Mais explicitamente, deve-se usar os comandos:
    g++ executa_quebra_senhas.cpp -std=c++17 -lstdc++ -pthread -o executa
    ./executa ./senhas/ ./decodificadas/

Com relação a estruturação do trabalho, eu fiz uma pergunta inicial para o usuário escolher se quer o uso de fork ou threads,
apos isso são criadas as estruturas escolhidas para cada arquivo e são executadas e armazenados os seus resultados em uma pasta.
Todo o código está comentado, espero que não esteja faltando nenhuma explicação.