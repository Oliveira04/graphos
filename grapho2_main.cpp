#include <iostream>
#include <fstream>
#include <string>   // Já estava implícito, mas bom ter
#include <vector>   // Para std::vector (lista de vizinhos)
#include <map>      // Para std::map (a lista de adjacência em si)
#include <sstream>  // Para "quebrar" as linhas facilmente
#include <algorithm> // Para std::sort (para ordenar os vizinhos)
#include <set>    // Para std::set (opcional, se precisar de conjuntos)

using namespace std;

void dfsRecursive(const string& currentNode,
                  const map<string, vector <string>>& graph,
                  std::map<string, bool>& visitedNodes,
                  std::vector<string>& visitationOrder) {

    visitedNodes[currentNode] = true;       
    visitationOrder.push_back(currentNode);
    if(graph.count(currentNode) ){
        const vector<string>&neighbors = graph.at(currentNode);
        for(const string& neighborNode : neighbors) {
            if(!visitedNodes.count(neighborNode) || !visitedNodes.at(neighborNode)) {
                dfsRecursive(neighborNode, graph, visitedNodes, visitationOrder);
            }
        }
    }
    
}

void startDFS(const string& startNode,
           const map<string, vector<string>>& graph,
           std::vector<string>& finalVisitationOrder) {
    std::map<string, bool> visitedNodesMap;
    finalVisitationOrder.clear();

    dfsRecursive(startNode, graph, visitedNodesMap, finalVisitationOrder);
}


int main() {
    ifstream g2_file("../g2.txt");

    if(!g2_file.is_open()){
        cerr << "Error: Could not open g2.txt" << endl;
        return 1;
    }
    map<string, vector<string>> adjacency_list;
    string line;
    cout << "Lendo o arquivo g2.txt e construindo a lista de adjacencia." << endl;

    while (getline(g2_file, line)) {
        stringstream ss(line);
        string vertex;
        string neighbor;

        if(getline(ss, vertex,';') && getline(ss, neighbor, ';')){
            adjacency_list[vertex].push_back(neighbor);
        }
        
        //cout << line << endl;
    }
    g2_file.close();
    cout << "Leitura de g2.txt concluida." << endl;
    for(auto& par : adjacency_list){
        sort(par.second.begin(), par.second.end()); // Ordena os vizinhos
    }
    cout << "Listas de vizinhos ordenadas." << endl << endl;
    cout << "Lista de adjacencia:" << endl;
    for(const auto& par: adjacency_list){
        cout << par.first << ":";
        for(const string&neighbor : par.second){
            cout << neighbor << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "Executando DFS em G2 partindo do vertice 'a'" << endl;
    vector<std::string> g2_dfs_visitation_order; // Vetor para guardar o resultado da DFS

    // Chama 'startDFS' passando a lista de adjacência de G2 e o vértice inicial 'a'
    startDFS("a", adjacency_list, g2_dfs_visitation_order);

    cout << "Ordem de visitacao da DFS em G2 partindo de 'a':" << endl;
    for (size_t i = 0; i < g2_dfs_visitation_order.size(); ++i) {
        cout << g2_dfs_visitation_order[i];
        if (i < g2_dfs_visitation_order.size() - 1) {
            cout << " -> "; // Para um formato de exibição visual
        }
    }
    cout << endl << endl;
    





    return 0;
}