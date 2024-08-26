Para poder executar o programa deve-se primeiro compilar os programas quebra_senhas.cpp e o executa_quebra_senhas.cpp
Após compilados deve se chamar o executavel de executa_quebra_senhas passando como parametros o nome do executavel de
quebra_senhas, o nome da pasta contendo as senhas e o nome da pasta onde ficarão os resultados
Mais explicitamente, deve-se usar os comandos:
    g++ quebra_senhas.cpp -std=c++17 -lstdc++ -pthread -o quebra_senhas
    g++ executa_quebra_senhas.cpp -std=c++17 -lstdc++ -o executa
    ./executa quebra_senhas ./senhas/ ./decodificadas/

Com relação a estruturação do trabalho, eu fiz uma pergunta inicial para o usuário escolher se quer o uso de fork ou threads,
apos isso são criadas as estruturas escolhidas para cada arquivo e são executadas e armazenados os seus resultados em uma pasta.
Todo o código está comentado, espero que não esteja faltando nenhuma explicação.