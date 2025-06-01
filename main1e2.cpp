// Trabalho da disciplina Analise de Algoritmos. Ministrado por: Prof Felipe Carvalho Pellison
//Alunos:
// 2136390 - Augusto Coimbra de Oliveira
// 2158087 - Joshua Lorenzo de Souza
//2125543 - Luis Felipe Rotondo Kobelnik


#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <tuple>
#include <queue>
#include <set>

using namespace std;

void mostrarListaAdjacencia(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cout << "nao foi possivel abrir" << endl;
        return;
    }

    map<string, vector<pair<string, string>>> adjacencia;
    string linha;

    while (getline(arquivo, linha)) {
        linha.erase(remove(linha.begin(), linha.end(), ' '), linha.end());
        if (linha.empty()) continue;

        size_t pos1 = linha.find(';');
        if (pos1 == string::npos) continue;
        size_t pos2 = linha.find(';', pos1 + 1);

        string origem = linha.substr(0, pos1);
        string destino, valor;
        if (pos2 != string::npos) {
            destino = linha.substr(pos1 + 1, pos2 - (pos1 + 1));
            valor = linha.substr(pos2 + 1);

            if (!valor.empty() && valor.back() == ';') {
                valor.pop_back();
            }
        } else {
            destino = linha.substr(pos1 + 1);
            valor = "1"; 
        }

        if (!origem.empty() && !destino.empty()) {

            if (nomeArquivo == "g2.txt") {
                adjacencia[origem].push_back({destino, valor});
            } else {
                adjacencia[origem].push_back({destino, valor});
                adjacencia[destino].push_back({origem, valor});
            }
        }
    }
    arquivo.close();


    for (auto& par : adjacencia) {
        sort(par.second.begin(), par.second.end());
        par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end());
    }

    cout << "Lista de adjacencia:" << endl;
    for (const auto& par : adjacencia) {
        cout << par.first << " --> ";
        for (size_t i = 0; i < par.second.size(); ++i) {
            if (nomeArquivo == "g3.txt") {
                cout << par.second[i].first << "(" << par.second[i].second << ")";
            } else {
                cout << par.second[i].first;
            }
            if (i < par.second.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

map<string, vector<string>> lerGrafoSemPeso(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    map<string, vector<string>> adj;
    string linha;

    while (getline(arquivo, linha)) {
        linha.erase(remove(linha.begin(), linha.end(), ' '), linha.end());
        if (linha.empty()) continue;
        size_t pos1 = linha.find(';');
        if (pos1 == string::npos) continue;
        string origem = linha.substr(0, pos1);
        string destino = linha.substr(pos1 + 1);
        if (!origem.empty() && !destino.empty()) {
            adj[origem].push_back(destino);
            adj[destino].push_back(origem); 
        }
    }
  
    for (auto& par : adj) {
        sort(par.second.begin(), par.second.end());
    }
    return adj;
}


void buscaEmLarguraG1() {
    auto adj = lerGrafoSemPeso("g1.txt");
    string inicio = "b";
    set<string> visitado;
    queue<string> fila;
    vector<string> ordem;

    fila.push(inicio);
    visitado.insert(inicio);

    while (!fila.empty()) {
        string atual = fila.front();
        fila.pop();
        ordem.push_back(atual);

        for (const auto& vizinho : adj[atual]) {
            if (visitado.find(vizinho) == visitado.end()) {
                fila.push(vizinho);
                visitado.insert(vizinho);
            }
        }
    }

    cout << "Ordem de visitacao: ";
    for (size_t i = 0; i < ordem.size(); ++i) {
        cout << ordem[i];
        if (i < ordem.size() - 1) cout << " -> ";
    }
    cout << endl;
}

int main() {
    int opcao;
    cout << "\nEscolha uma opcao:\n";
    cout << "1 - Ler arquivo e criar lista de adjacencia\n";
    cout << "2 - Busca em largura no grafo 1 a partir do vertice B\n";
    cout << "3 - Exercicio 3\n";
    cout << "4 - Exercicio 4\n";
    cout << "0 - Sair\n";
    cout << "Digite o numero da opcao desejada: ";
    cin >> opcao;
    cin.ignore();

    switch (opcao) {
        case 1: { // Exercicio 1
            int subopcao;
            do {
                cout << "\nMenu da opcao 1:\n";
                cout << "1 - Mostrar lista de adjacencia de g1.txt\n";
                cout << "2 - Mostrar lista de adjacencia de g2.txt\n";
                cout << "3 - Mostrar lista de adjacencia de g3.txt\n";
                cout << "0 - Sair\n";
                cout << "Digite o numero da subopcao desejada: ";
                cin >> subopcao;
                cin.ignore();
                switch (subopcao) {
                    case 1:
                        mostrarListaAdjacencia("g1.txt");
                        break;
                    case 2:
                        mostrarListaAdjacencia("g2.txt");
                        break;
                    case 3:
                        mostrarListaAdjacencia("g3.txt");
                        break;
                    case 0:
                        cout << "Obrigado por participar, espero que tenha gostado! :D" << endl;
                        break;
                    default:
                        cout << "Subopcao invalida!" << endl;
                }
            } while (subopcao != 0);
            break;
        }
        case 2: { // Exercicio 2
            buscaEmLarguraG1();
            break;
        }
        case 3: { // Exercicio 3
            cout << "Essa parte do código ainda não foi implementada, por favor aguarde um pouco." << endl;
            break;
        }
        case 4: { // Exercicio 4
            cout << "Essa parte do código ainda não foi implementada, por favor aguarde um pouco." << endl;
            break;
        }
        case 0:
            cout << "Obrigado por participar! :D" << endl;
            return 0;
        default:
            cout << "Opcao invalida!" << endl;
    }
}