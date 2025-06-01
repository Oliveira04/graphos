// Trabalho da disciplina Analise de Algoritmos. Ministrado por: Prof Felipe Carvalho Pellison
//Alunos:
// 2136390 - Augusto Coimbra de Oliveira
// 2158087 - Joshua Lorenzo de Souza
//2125543 - Luis Felipe Rotondo Kobelnik

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

// Responsável por abrir o arquivo e obter os dados
void readGraph(const string& filename, map<string, vector<pair<string, int>>>& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        replace(line.begin(), line.end(), ';', ' ');
        stringstream ss(line);
        string from, to;
        int weight;
        ss >> from >> to >> weight;

        graph[from].push_back(make_pair(to, weight));
        graph[to].push_back(make_pair(from, weight));
    }

    file.close();
}

// Exibe o grafo no console (Pra debugar)
void printGraph(const map<string, vector<pair<string, int>>>& graph) {
    map<string, vector<pair<string, int>>>::const_iterator it;
    for (it = graph.begin(); it != graph.end(); ++it) {
        const string& from = it->first;
        const vector<pair<string, int>>& edges = it->second;

        cout << from << " -> ";
        for (size_t i = 0; i < edges.size(); ++i) {
            const string& to = edges[i].first;
            int weight = edges[i].second;
            cout << "(" << to << ", " << weight << ") ";
        }
        cout << endl;
    }
}

// Faz o dijkstra
int dijkstra(const map<string, vector<pair<string, int>>>& graph, const string& origem, const string& destino, vector<string>& caminho) {
    map<string, int> dist;
    map<string, string> predecessor;
    for (auto& p : graph) {
        dist[p.first] = numeric_limits<int>::max();
    }
    dist[origem] = 0;

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    pq.push({0, origem});

    while (!pq.empty()) {
        auto [distAtual, u] = pq.top();
        pq.pop();

        if (u == destino) {
            // Reconstruir caminho
            string atual = destino;
            while (atual != origem) {
                caminho.push_back(atual);
                atual = predecessor[atual];
            }
            caminho.push_back(origem);
            reverse(caminho.begin(), caminho.end());
            return distAtual;
        }

        if (distAtual > dist[u]) continue;

        for (const auto& [v, peso] : graph.at(u)) {
            int novaDist = distAtual + peso;
            if (novaDist < dist[v]) {
                dist[v] = novaDist;
                predecessor[v] = u;
                pq.push({novaDist, v});
            }
        }
    }

    return -1;  // Não tem conexão entre os pontos
}

int main() {
    map<string, vector<pair<string, int>>> graph;
    readGraph("g3.txt", graph);

    cout << "Grafo lido:\n";
    printGraph(graph);

    string origem = "x";
    string destino = "t";

    cout << "\nExecutando Dijkstra de " << origem << " ao " << destino << "...\n";

    vector<string> caminho;
    int distancia = dijkstra(graph, origem, destino, caminho);

    if (distancia != -1) {
        cout << "A menor distancia de " << origem << " ao " << destino << " resulta em: " << distancia << endl;
        cout << "Caminho: ";
        for (size_t i = 0; i < caminho.size(); ++i) {
            cout << caminho[i];
            if (i != caminho.size() - 1) cout << " -> ";
        }
        cout << endl;
    } else {
        cout << "Não existe caminho de " << origem << " ao " << destino << endl;
    }

    return 0;
}