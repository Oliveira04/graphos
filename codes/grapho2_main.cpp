//esse vai ser o arquivo principal do trabalho, na qual será usado para receber e processar as funções dos outros arquivos
//vizando que a separeção serve para organizar melhor o código e facilitar a manutenção


#include <iostream>
#include <fstream>
#include "open_file.h" // <<< INCLUA O HEADER DA SUA FUNÇÃO
#include <string>
using namespace std;




int main() {
   std::cout << "Programa principal iniciado." << std::endl;

    processLine(); // <<< CHAMANDO A FUNÇÃO DO OUTRO ARQUIVO

    std::cout << "Programa principal finalizado." << std::endl;
    return 0;
}