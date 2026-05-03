#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Graph {
private:
    int n;
    vector<vector<int>> adj;
    vector<vector<int>> matrix;
    vector<bool> visited;

public:
    Graph(int vertices) {
        n = vertices;
        adj.resize(n);
        matrix.resize(n, vector<int>(n, 0));
        visited.resize(n, false);
    }

    void addEdge(int u, int v, int weight = 1) {
        adj[u].push_back(v);
        adj[v].push_back(u);

        matrix[u][v] = weight;
        matrix[v][u] = weight;
    }

    void SpanningTree() {
        fill(visited.begin(), visited.end(), false);

        vector<vector<int>> copyAdj = adj;
        vector<pair<int, int>> treeEdges;

        SpanningTreeDFS(0, copyAdj, treeEdges);

        cout << "Spanning Tree Edges:" << endl;

        for (auto e : treeEdges) {
            cout << e.first << " - " << e.second << endl;
        }

        vector<pair<int, int>> backupTreeEdges = treeEdges;
    }

    void SpanningTreeDFS(
        int v,
        vector<vector<int>> copyAdj,
        vector<pair<int, int>>& treeEdges
    ) {
        visited[v] = true;

        vector<int> neighbors = copyAdj[v];

        for (int w : neighbors) {
            if (!visited[w]) {
                treeEdges.push_back({v, w});
                SpanningTreeDFS(w, copyAdj, treeEdges);
            }
        }
    }
};

int main() {
    Graph g(6);

    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 2);
    g.addEdge(1, 3, 5);
    g.addEdge(3, 4, 1);
    g.addEdge(4, 5, 6);

    cout << "===== Spanning Tree =====" << endl;
    g.SpanningTree();

    return 0;
}