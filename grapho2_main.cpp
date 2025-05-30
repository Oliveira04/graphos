// Trabalho da disciplina Analise de Algoritmos. Ministrado por: Prof Felipe Carvalho Pellison
//Alunos:
// 2136390 - Augusto Coimbra de Oliveira
// 2158087 - Joshua Lorenzo de Souza
//2125543 - Luis Felipe Rotondo Kobelnik

// Inclusão das bibliotecas padrão do C++ necessárias para o programa
#include <iostream>  // Para entrada e saída de dados no console (std::cout, std::cerr, std::endl)
#include <fstream>   // Para manipulação de arquivos (std::ifstream para ler arquivos)
#include <string>    // Para usar a classe std::string (manipulação de texto)
#include <vector>    // Para usar a classe std::vector (arrays dinâmicos, usados para listas de vizinhos)
#include <map>       // Para usar a classe std::map (contêiner associativo chave-valor, usado para a lista de adjacência)
#include <sstream>   // Para usar std::stringstream (tratar strings como fluxos de dados para parsing)
#include <algorithm> // Para usar algoritmos como std::sort (ordenar os vizinhos)
#include <set>       // Para usar std::set (opcional, como alternativa para marcar nós visitados)

using namespace std; 

/**
 * @brief Realiza a busca em profundidade (DFS) de forma recursiva.
 *
 * @param currentNode O nó (vértice) atual que está sendo visitado/explorado.
 * @param graph A lista de adjacência do grafo (constante, pois não é modificada pela DFS).
 * @param visitedNodes Mapa para rastrear os nós já visitados (passado por referência para ser atualizado).
 * @param visitationOrder Vetor para armazenar a ordem em que os nós são visitados pela primeira vez (passado por referência).
 */
void dfsRecursive(const string& currentNode,
                  const map<string, vector<string>>& graph,
                  map<string, bool>& visitedNodes,
                  vector<string>& visitationOrder) {

    // Imprime uma mensagem indicando a entrada na função para o nó atual (para depuração/visualização)
    cout << "DFS: Entrando no vertice: " << currentNode << endl;

    // Marca o nó atual como visitado no mapa de nós visitados.
    visitedNodes[currentNode] = true;
    // Adiciona o nó atual ao vetor que registra a ordem final de visitação.
    visitationOrder.push_back(currentNode);

    // Imprime o estado atual (para depuração/visualização)
    cout << "   -> Marcado " << currentNode << " como visitado. Ordem atual: ";
    for (size_t i = 0; i < visitationOrder.size(); ++i) {
        cout << visitationOrder[i] << (i == visitationOrder.size() - 1 ? "" : ", ");
    }
    cout << endl;

    // Verifica se o nó atual ('currentNode') possui uma entrada na lista de adjacência 'graph'.
    // Isso é importante porque um nó pode ser destino de uma aresta, mas não ter arestas de saída.
    if (graph.count(currentNode)) {
        // Obtém uma referência constante à lista de vizinhos do nó atual.
        // A lista de vizinhos já deve estar ordenada lexicograficamente.
        // Usar 'const &' evita uma cópia desnecessária do vetor de vizinhos.
        const vector<string>& neighbors = graph.at(currentNode);

        // Imprime os vizinhos do nó atual (para depuração/visualização)
        cout << "   -> Vizinhos de " << currentNode << " (em ordem): ";
        for (const string& n : neighbors) cout << n << " ";
        cout << endl;

        // Itera sobre cada vizinho ('neighborNode') do 'currentNode'.
        for (const string& neighborNode : neighbors) {
            cout << "      Considerando vizinho: " << neighborNode << " de " << currentNode << endl;
            // Verifica se o 'neighborNode' ainda NÃO foi visitado.
            // A condição '!visitedNodes.count(neighborNode)' checa se o vizinho nem está no mapa de visitados.
            // Se estiver, '!visitedNodes.at(neighborNode)' checa se seu valor é 'false'.
            if (!visitedNodes.count(neighborNode) || !visitedNodes.at(neighborNode)) {
                cout << "         -> " << neighborNode << " nao visitado. Chamando DFS recursivamente." << endl;
                // Se não foi visitado, faz uma chamada recursiva para 'dfsRecursive'
                // começando a partir deste 'neighborNode'.
                dfsRecursive(neighborNode, graph, visitedNodes, visitationOrder);
                // Após o retorno da chamada recursiva (backtracking), imprime uma mensagem (para depuração/visualização)
                cout << "DFS: Retornou da chamada recursiva para " << neighborNode << ", de volta em " << currentNode << endl;
            } else {
                // Se o vizinho já foi visitado, imprime uma mensagem (para depuração/visualização)
                cout << "         -> " << neighborNode << " JA VISITADO." << endl;
            }
        }
    } else {
        // Se o 'currentNode' não tem vizinhos de saída (não é chave no mapa 'graph')
        cout << "   -> " << currentNode << " nao tem vizinhos de saida na lista de adjacencia." << endl;
    }
    // Imprime uma mensagem indicando que a exploração a partir do 'currentNode' terminou (para depuração/visualização)
    cout << "DFS: Saindo do vertice (fim da exploracao a partir de): " << currentNode << endl;
}

/**
 * @brief Função auxiliar para inicializar e iniciar o processo de DFS.
 *
 * @param startNode O nó (vértice) a partir do qual a DFS deve começar.
 * @param graph A lista de adjacência do grafo.
 * @param finalVisitationOrder Vetor que armazenará a ordem final de visitação dos nós.
 */
void startDFS(const string& startNode,
              const map<string, vector<string>>& graph,
              std::vector<string>& finalVisitationOrder) {
    // Cria um mapa local para rastrear os nós visitados especificamente para esta execução da DFS.
    std::map<string, bool> visitedNodesMap;
    // Limpa o vetor de resultado para garantir que não contenha dados de execuções anteriores.
    finalVisitationOrder.clear();

    // Chama a função recursiva principal da DFS.
    dfsRecursive(startNode, graph, visitedNodesMap, finalVisitationOrder);
}

// Função principal do programa
int main() {
    // Abre o arquivo "g2.txt" para leitura. "../" indica que o arquivo está no diretório pai.
    ifstream g2_file("../g2.txt");

    // Verifica se o arquivo foi aberto corretamente.
    if (!g2_file.is_open()) {
        cerr << "Error: Could not open g2.txt" << endl; // Mensagem de erro para o fluxo de erro padrão
        return 1; // Retorna 1 para indicar que o programa terminou com erro
    }

    // Declara a lista de adjacência para G2.
    map<string, vector<string>> adjacency_list;
    string line; // String para armazenar cada linha lida do arquivo
    cout << "Lendo o arquivo g2.txt e construindo a lista de adjacencia." << endl;

    // Loop para ler o arquivo linha por linha
    while (getline(g2_file, line)) {
        stringstream ss(line); // Usa stringstream para facilitar o parsing da linha
        string vertex;   // Armazena o vértice de origem
        string neighbor; // Armazena o vértice de destino

        // Extrai 'vertex' e 'neighbor' da linha, usando ';' como delimitador
        if (getline(ss, vertex, ';') && getline(ss, neighbor, ';')) {
            // Adiciona 'neighbor' à lista de vizinhos de 'vertex'.
            // Correto para grafo direcionado G2 (vertex -> neighbor).
            adjacency_list[vertex].push_back(neighbor);
        }
    }
    g2_file.close(); // Fecha o arquivo após a leitura
    cout << "Leitura de g2.txt concluida." << endl;

    // Itera sobre cada entrada da lista de adjacência para ordenar os vizinhos.
    // 'auto& par' permite modificar o vetor de vizinhos diretamente.
    for (auto& par : adjacency_list) {
        // Ordena o vetor de vizinhos (par.second) em ordem lexicográfica.
        // Importante para a exigência do trabalho sobre a ordem de visitação.
        sort(par.second.begin(), par.second.end());
    }
    cout << "Listas de vizinhos ordenadas." << endl << endl;

    // (Opcional) Imprime a lista de adjacência construída para verificação.
    cout << "Lista de adjacencia:" << endl;
    for (const auto& par : adjacency_list) { // 'const auto&' para apenas leitura
        cout << par.first << ":"; // Imprime o vértice
        for (const string& current_neighbor : par.second) { // Itera sobre os vizinhos
            cout << current_neighbor << " "; // Imprime cada vizinho
        }
        cout << endl;
    }
    cout << endl;

    // Executa a Busca em Profundidade (DFS) para o Grafo G2
    cout << "Executando DFS em G2 partindo do vertice 'a'" << endl;
    vector<std::string> g2_dfs_visitation_order; // Vetor para armazenar o resultado da DFS

    // Chama a função 'startDFS' para iniciar a busca.
    // "a" é o nó inicial, 'adjacency_list' é o grafo G2,
    // e 'g2_dfs_visitation_order' receberá a ordem de visitação.
    startDFS("a", adjacency_list, g2_dfs_visitation_order);

    // Imprime a ordem de visitação final da DFS, conforme solicitado pela Tarefa 3.
    cout << "Ordem de visitacao da DFS em G2 partindo de 'a':" << endl;
    for (size_t i = 0; i < g2_dfs_visitation_order.size(); ++i) {
        cout << g2_dfs_visitation_order[i];
        if (i < g2_dfs_visitation_order.size() - 1) {
            cout << " -> "; // Adiciona "->" entre os nós para melhor visualização
        }
    }
    cout << endl << endl;

    // O programa termina com sucesso.
    return 0;
}