// Trabalho da disciplina Analise de Algoritmos. Ministrado por: Prof Felipe Carvalho Pellison
// Alunos:
// 2136390 - Augusto Coimbra de Oliveira
// 2158087 - Joshua Lorenzo de Souza
// 2125543 - Luis Felipe Rotondo Kobelnik

#include <iostream>     // For std::cout, std::cerr, std::endl, std::cin
#include <fstream>      // For std::ifstream (file input)
#include <string>       // For std::string
#include <vector>       // For std::vector
#include <map>          // For std::map
#include <algorithm>    // For std::sort, std::remove, std::unique, std::reverse
#include <queue>        // For std::queue (BFS) and std::priority_queue (Dijkstra)
#include <set>          // For std::set (BFS visited)
#include <sstream>      // For std::stringstream (parsing lines)
#include <limits>       // For std::numeric_limits (Dijkstra infinity)
// No need for <tuple> as std::pair is used directly

using namespace std;

// --- Estruturas e Typedefs (se necessário, mas aqui map/vector/pair são diretos) ---

// --- Funções do Snippet 1 (Adjacency List Display & BFS for G1) ---

void mostrarListaAdjacencia(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cout << "Nao foi possivel abrir o arquivo: " << nomeArquivo << endl;
        return;
    }

    map<string, vector<pair<string, string>>> adjacencia;
    string linha;

    while (getline(arquivo, linha)) {
        // Remove all spaces from the line first (simplifies parsing if spaces are inconsistent)
        linha.erase(remove(linha.begin(), linha.end(), ' '), linha.end());
        if (linha.empty()) continue;

        size_t pos1 = linha.find(';');
        if (pos1 == string::npos) {
            // cerr << "Warning: Linha mal formatada (sem ';'): " << linha << endl; // Optional warning
            continue;
        }
        size_t pos2 = linha.find(';', pos1 + 1);

        string origem = linha.substr(0, pos1);
        string destino, valor;

        if (pos2 != string::npos) { // Formato: origem;destino;valor
            destino = linha.substr(pos1 + 1, pos2 - (pos1 + 1));
            valor = linha.substr(pos2 + 1);
            // Remove trailing semicolon from valor if present
            if (!valor.empty() && valor.back() == ';') {
                valor.pop_back();
            }
        } else { // Formato: origem;destino (sem valor explícito)
            destino = linha.substr(pos1 + 1);
            valor = "1"; // Valor padrão para grafos não ponderados ou quando o peso é implícito
        }
        
        // Remove trailing semicolon from destino if it was the last char and no valor
        if (!destino.empty() && destino.back() == ';') {
            destino.pop_back();
        }


        if (!origem.empty() && !destino.empty()) {
            if (nomeArquivo == "g2.txt") { // G2 é direcionado: origem -> destino
                adjacencia[origem].push_back({destino, valor});
            } else { // G1 e G3 são não direcionados (ou tratados como tal por esta função)
                adjacencia[origem].push_back({destino, valor});
                adjacencia[destino].push_back({origem, valor}); // Adiciona a aresta reversa
            }
        }
    }
    arquivo.close();

    // Ordena e remove duplicatas para cada lista de adjacência
    for (auto& par : adjacencia) {
        sort(par.second.begin(), par.second.end());
        par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end());
    }

    cout << "Lista de adjacencia para " << nomeArquivo << ":" << endl;
    for (const auto& par : adjacencia) {
        cout << par.first << " --> ";
        for (size_t i = 0; i < par.second.size(); ++i) {
            // Para g3.txt (ponderado) ou qualquer arquivo com valor, mostrar o valor.
            // Para outros, só o destino se o valor for "1" (padrão).
            // A condição original era específica para g3.txt, vamos generalizar um pouco
            // para mostrar peso se ele não for o padrão "1" ou se for g3.txt
            if (nomeArquivo == "g3.txt" || (par.second[i].second != "1" && !par.second[i].second.empty()) ) {
                 cout << par.second[i].first << "(" << par.second[i].second << ")";
            } else {
                 cout << par.second[i].first;
            }

            if (i < par.second.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

map<string, vector<string>> lerGrafoSemPesoG1(const string& nomeArquivo) { // Específico para G1 (não direcionado, sem peso)
    ifstream arquivo(nomeArquivo);
    map<string, vector<string>> adj;
    string linha;

    if (!arquivo.is_open()) {
        cerr << "Nao foi possivel abrir o arquivo: " << nomeArquivo << endl;
        return adj; // Retorna grafo vazio
    }

    while (getline(arquivo, linha)) {
        linha.erase(remove(linha.begin(), linha.end(), ' '), linha.end());
        if (linha.empty()) continue;
        size_t pos1 = linha.find(';');
        if (pos1 == string::npos) continue;
        
        string origem = linha.substr(0, pos1);
        string destino = linha.substr(pos1 + 1);
        // Remove trailing semicolon from destino if present
        if (!destino.empty() && destino.back() == ';') {
            destino.pop_back();
        }

        if (!origem.empty() && !destino.empty()) {
            adj[origem].push_back(destino);
            adj[destino].push_back(origem); // Não direcionado
        }
    }
    arquivo.close();
 
    for (auto& par : adj) {
        sort(par.second.begin(), par.second.end()); // Ordena vizinhos para BFS determinístico
        par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); // Remove duplicatas
    }
    return adj;
}

void buscaEmLarguraG1() {
    cout << "\n--- Executando Busca em Largura em G1 (g1.txt) a partir de 'b' ---" << endl;
    auto adj = lerGrafoSemPesoG1("g1.txt");
    if (adj.empty() && !ifstream("g1.txt").good()){ // Check if graph is empty because file couldn't be read
        cout << "Falha ao ler g1.txt. Abortando BFS." << endl;
        return;
    }
    if (adj.find("b") == adj.end()) {
        cout << "Vertice de inicio 'b' nao encontrado no grafo g1.txt." << endl;
        return;
    }


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

        // Verifica se 'atual' existe no mapa antes de acessar seus vizinhos
        if (adj.count(atual)) {
            for (const auto& vizinho : adj.at(atual)) {
                if (visitado.find(vizinho) == visitado.end()) {
                    fila.push(vizinho);
                    visitado.insert(vizinho);
                }
            }
        }
    }

    cout << "Ordem de visitacao (BFS): ";
    for (size_t i = 0; i < ordem.size(); ++i) {
        cout << ordem[i];
        if (i < ordem.size() - 1) cout << " -> ";
    }
    cout << endl << endl;
}

// --- Funções do Snippet 2 (DFS for G2) ---

map<string, vector<string>> lerGrafoG2ParaDFS(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    map<string, vector<string>> adj;
    string linha;

    if (!arquivo.is_open()) {
        cerr << "Nao foi possivel abrir o arquivo: " << nomeArquivo << endl;
        return adj;
    }
    
    cout << "Lendo o arquivo " << nomeArquivo << " e construindo a lista de adjacencia para DFS." << endl;
    while (getline(arquivo, linha)) {
        // Não remover espaços aqui, pois stringstream lida com eles
        stringstream ss(linha);
        string vertice;
        string vizinho;
        // Formato esperado: vertice;vizinho; (ou vertice;vizinho)
        if (getline(ss, vertice, ';') && getline(ss, vizinho, ';')) {
            // Remover espaços que possam ter sido lidos por getline
            vertice.erase(remove(vertice.begin(), vertice.end(), ' '), vertice.end());
            vizinho.erase(remove(vizinho.begin(), vizinho.end(), ' '), vizinho.end());

            if (!vertice.empty() && !vizinho.empty()) {
                adj[vertice].push_back(vizinho); // G2 é direcionado
            }
        }
    }
    arquivo.close();
    cout << "Leitura de " << nomeArquivo << " concluida." << endl;

    for (auto& par : adj) {
        sort(par.second.begin(), par.second.end()); // Ordena vizinhos
        par.second.erase(unique(par.second.begin(), par.second.end()), par.second.end()); // Remove duplicatas
    }
    cout << "Listas de vizinhos ordenadas para DFS." << endl << endl;
    return adj;
}


void dfsRecursive(const string& currentNode,
                  const map<string, vector<string>>& graph,
                  map<string, bool>& visitedNodes,
                  vector<string>& visitationOrder,
                  bool verbose = false) { // Added verbose flag

    if (verbose) cout << "DFS: Entrando no vertice: " << currentNode << endl;
    visitedNodes[currentNode] = true;
    visitationOrder.push_back(currentNode);

    if (verbose) {
        cout << "   -> Marcado " << currentNode << " como visitado. Ordem atual: ";
        for (size_t i = 0; i < visitationOrder.size(); ++i) {
            cout << visitationOrder[i] << (i == visitationOrder.size() - 1 ? "" : ", ");
        }
        cout << endl;
    }

    if (graph.count(currentNode)) {
        const vector<string>& neighbors = graph.at(currentNode);
        if (verbose) {
            cout << "   -> Vizinhos de " << currentNode << " (em ordem): ";
            for (const string& n : neighbors) cout << n << " ";
            cout << endl;
        }

        for (const string& neighborNode : neighbors) {
            if (verbose) cout << "        Considerando vizinho: " << neighborNode << " de " << currentNode << endl;
            if (!visitedNodes.count(neighborNode) || !visitedNodes.at(neighborNode)) {
                if (verbose) cout << "           -> " << neighborNode << " nao visitado. Chamando DFS recursivamente." << endl;
                dfsRecursive(neighborNode, graph, visitedNodes, visitationOrder, verbose);
                if (verbose) cout << "DFS: Retornou da chamada recursiva para " << neighborNode << ", de volta em " << currentNode << endl;
            } else {
                if (verbose) cout << "           -> " << neighborNode << " JA VISITADO." << endl;
            }
        }
    } else {
        if (verbose) cout << "   -> " << currentNode << " nao tem vizinhos de saida na lista de adjacencia." << endl;
    }
    if (verbose) cout << "DFS: Saindo do vertice (fim da exploracao a partir de): " << currentNode << endl;
}

void startDFS(const string& startNode,
              const map<string, vector<string>>& graph,
              vector<string>& finalVisitationOrder,
              bool verbose = false) { // Added verbose flag
    map<string, bool> visitedNodesMap;
    finalVisitationOrder.clear();
    
    if (graph.find(startNode) == graph.end() && !graph.empty()) { // Check if startNode exists
        bool foundAsNeighbor = false;
        for(const auto& pair : graph){
            for(const auto& neighbor : pair.second){
                if(neighbor == startNode){
                    foundAsNeighbor = true;
                    break;
                }
            }
            if(foundAsNeighbor) break;
        }
        if(!foundAsNeighbor && !graph.count(startNode)){ // if not a source node and not a neighbor node, it might not exist at all
             bool presentAtAll = false;
             for(const auto& pair_ : graph) { // check if it is a key
                if (pair_.first == startNode) {
                    presentAtAll = true;
                    break;
                }
                for(const auto& neighbor : pair_.second) { // check if it is a value in any neighbor list
                    if (neighbor == startNode) {
                        presentAtAll = true;
                        break;
                    }
                }
                if (presentAtAll) break;
            }
            if (!presentAtAll) {
                cout << "Vertice de inicio '" << startNode << "' nao encontrado no grafo. Abortando DFS." << endl;
                return;
            }
        }
    }


    dfsRecursive(startNode, graph, visitedNodesMap, finalVisitationOrder, verbose);
}

void executarDFS_G2() {
    cout << "\n--- Executando Busca em Profundidade em G2 (g2.txt) a partir de 'a' ---" << endl;
    map<string, vector<string>> graph_g2 = lerGrafoG2ParaDFS("g2.txt");

    if (graph_g2.empty() && !ifstream("g2.txt").good()){
         cout << "Falha ao ler g2.txt ou grafo vazio. Abortando DFS." << endl;
        return;
    }
    
    // (Opcional) Imprime a lista de adjacência construída para verificação.
    cout << "Lista de adjacencia de G2 (para DFS):" << endl;
    for (const auto& par : graph_g2) {
        cout << par.first << " --> ";
        for (size_t i = 0; i < par.second.size(); ++i) {
            cout << par.second[i];
            if (i < par.second.size() - 1) cout << ", ";
        }
        cout << endl;
    }
    cout << endl;

    vector<string> g2_dfs_visitation_order;
    // Set verbose to true for detailed DFS steps, false for just the final order
    startDFS("a", graph_g2, g2_dfs_visitation_order, true); 

    if (g2_dfs_visitation_order.empty() && (graph_g2.empty() || graph_g2.find("a") == graph_g2.end())) {
        // handled in startDFS or lerGrafoG2ParaDFS
    } else {
        cout << "Ordem de visitacao (DFS) em G2 partindo de 'a':" << endl;
        for (size_t i = 0; i < g2_dfs_visitation_order.size(); ++i) {
            cout << g2_dfs_visitation_order[i];
            if (i < g2_dfs_visitation_order.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl << endl;
    }
}


// --- Funções do Snippet 3 (Dijkstra for G3) ---

// Renamed to be specific for G3 and its format (undirected, integer weights)
void lerGrafoG3ParaDijkstra(const string& filename, map<string, vector<pair<string, int>>>& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        // exit(1); // Prefer to return and let main handle, or just print error
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Replace ';' with space for stringstream parsing
        replace(line.begin(), line.end(), ';', ' ');
        stringstream ss(line);
        string from, to;
        int weight;
        
        if (ss >> from >> to >> weight) { // Ensures all three are read
            graph[from].push_back(make_pair(to, weight));
            graph[to].push_back(make_pair(from, weight)); // Undirected graph
        } else {
            // cerr << "Warning: Linha mal formatada em " << filename << ": " << line << endl; // Optional
        }
    }
    file.close();

    // Sort neighbors for consistent behavior if needed (not strictly for Dijkstra's correctness but good practice)
    for (auto& pair_ : graph) {
        sort(pair_.second.begin(), pair_.second.end());
    }
}

// Renamed to be specific
void printGrafoG3(const map<string, vector<pair<string, int>>>& graph) {
    cout << "Grafo G3 (para Dijkstra):" << endl;
    // Using const_iterator for practice, though range-based for is often cleaner
    map<string, vector<pair<string, int>>>::const_iterator it;
    for (it = graph.begin(); it != graph.end(); ++it) {
        const string& from = it->first;
        const vector<pair<string, int>>& edges = it->second;

        cout << from << " --> ";
        for (size_t i = 0; i < edges.size(); ++i) {
            const string& to = edges[i].first;
            int weight = edges[i].second;
            cout << "(" << to << ", " << weight << ")";
            if (i < edges.size() - 1) cout << " ";
        }
        cout << endl;
    }
}

int dijkstra(const map<string, vector<pair<string, int>>>& graph, const string& origem, const string& destino, vector<string>& caminho) {
    map<string, int> dist;
    map<string, string> predecessor;
    // Initialize distances to infinity for all potential nodes
    // Collect all unique nodes first
    set<string> nodes;
    for(const auto& pair_ : graph) {
        nodes.insert(pair_.first);
        for(const auto& edge : pair_.second) {
            nodes.insert(edge.first);
        }
    }

    for (const string& node : nodes) {
        dist[node] = numeric_limits<int>::max();
    }

    if (nodes.find(origem) == nodes.end()) {
        cout << "Vertice de origem '" << origem << "' nao existe no grafo." << endl;
        return -1;
    }
    dist[origem] = 0;

    // Min-priority queue: stores {distance, vertex_name}
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    pq.push({0, origem});

    while (!pq.empty()) {
        int distAtual = pq.top().first; // current distance from source
        string u = pq.top().second;     // current vertex
        pq.pop();

        // If path to u is already shorter or u is already processed with shorter path
        if (distAtual > dist[u]) {
            continue;
        }

        if (u == destino) { // Destination reached
            string atual = destino;
            while (predecessor.count(atual)) { // Check if 'atual' has a predecessor (origem won't)
                caminho.push_back(atual);
                atual = predecessor[atual];
            }
            caminho.push_back(origem); // Add the origin
            reverse(caminho.begin(), caminho.end());
            return dist[destino]; // or distAtual
        }

        // If 'u' is not in the graph (e.g. isolated node mentioned but no edges from it)
        // or it has no outgoing edges
        if (graph.find(u) == graph.end()) {
            continue;
        }

        // For each neighbor v of u
        for (const auto& edge : graph.at(u)) {
            string v = edge.first;
            int peso = edge.second;
            if (dist.find(v) == dist.end()) dist[v] = numeric_limits<int>::max(); // Ensure neighbor is in dist map

            if (dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                predecessor[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return -1; // Destino não alcançável
}

void executarDijkstraG3() {
    cout << "\n--- Executando Algoritmo de Dijkstra em G3 (g3.txt) ---" << endl;
    map<string, vector<pair<string, int>>> graph_g3;
    lerGrafoG3ParaDijkstra("g3.txt", graph_g3);

    if (graph_g3.empty() && !ifstream("g3.txt").good()){
        cout << "Falha ao ler g3.txt ou grafo vazio. Abortando Dijkstra." << endl;
        return;
    }

    printGrafoG3(graph_g3);

    string origem_dijkstra = "x"; // Pode ser alterado ou solicitado ao usuário
    string destino_dijkstra = "t"; // Pode ser alterado ou solicitado ao usuário

    cout << "\nCalculando menor caminho de '" << origem_dijkstra << "' para '" << destino_dijkstra << "'..." << endl;

    vector<string> caminho_dijkstra;
    int distancia = dijkstra(graph_g3, origem_dijkstra, destino_dijkstra, caminho_dijkstra);

    if (distancia != -1) {
        cout << "Menor distancia de " << origem_dijkstra << " para " << destino_dijkstra << ": " << distancia << endl;
        cout << "Caminho: ";
        for (size_t i = 0; i < caminho_dijkstra.size(); ++i) {
            cout << caminho_dijkstra[i];
            if (i != caminho_dijkstra.size() - 1) cout << " -> ";
        }
        cout << endl;
    } else {
        cout << "Nao existe caminho de " << origem_dijkstra << " para " << destino_dijkstra << "." << endl;
    }
    cout << endl;
}


// --- Função Principal (main) ---
int main() {
    int opcao;
    do {
        cout << "\n========= MENU PRINCIPAL =========" << endl;
        cout << "Trabalho de Analise de Algoritmos" << endl;
        cout << "Alunos: Augusto, Joshua, Luis Felipe" << endl;
        cout << "------------------------------------" << endl;
        cout << "1 - Exercicio 1: Ler arquivo e criar lista de adjacencia" << endl;
        cout << "2 - Exercicio 2: Busca em Largura (BFS) no Grafo G1 (g1.txt) a partir de 'b'" << endl;
        cout << "3 - Exercicio 3: Busca em Profundidade (DFS) no Grafo G2 (g2.txt) a partir de 'a'" << endl;
        cout << "4 - Exercicio 4: Algoritmo de Dijkstra no Grafo G3 (g3.txt) de 'x' para 't'" << endl;
        cout << "0 - Sair" << endl;
        cout << "------------------------------------" << endl;
        cout << "Digite o numero da opcao desejada: ";
        
        cin >> opcao;
        if (cin.fail()) { // Handle invalid input (e.g., char instead of int)
            cout << "Entrada invalida! Por favor, digite um numero." << endl;
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            opcao = -1; // Set to an invalid option to loop again
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consome o newline deixado pelo cin >> opcao

        switch (opcao) {
            case 1: {
                int subopcao;
                do {
                    cout << "\n--- Menu Exercicio 1: Listas de Adjacencia ---" << endl;
                    cout << "1 - Mostrar lista de adjacencia de g1.txt" << endl;
                    cout << "2 - Mostrar lista de adjacencia de g2.txt (Direcionado)" << endl;
                    cout << "3 - Mostrar lista de adjacencia de g3.txt (Ponderado)" << endl;
                    cout << "0 - Voltar ao menu principal" << endl;
                    cout << "Digite o numero da subopcao desejada: ";
                    cin >> subopcao;
                    if (cin.fail()) {
                        cout << "Entrada invalida! Por favor, digite um numero." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        subopcao = -1;
                        continue;
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

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
                            cout << "Retornando ao menu principal..." << endl;
                            break;
                        default:
                            cout << "Subopcao invalida!" << endl;
                    }
                } while (subopcao != 0);
                break;
            }
            case 2: {
                buscaEmLarguraG1();
                break;
            }
            case 3: {
                executarDFS_G2();
                break;
            }
            case 4: {
                executarDijkstraG3();
                break;
            }
            case 0:
                cout << "Obrigado por participar! :D" << endl;
                break;
            default:
                cout << "Opcao invalida! Tente novamente." << endl;
        }
    } while (opcao != 0);

    return 0;
}