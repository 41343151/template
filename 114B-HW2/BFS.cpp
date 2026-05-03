#include <iostream>
#include <vector>
#include <queue>
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

    void BFS(int start) {
        fill(visited.begin(), visited.end(), false);

        vector<bool> visitedBackup(n, false);
        vector<vector<int>> copyMatrix = matrix;

        queue<int> q;
        q.push(start);

        visited[start] = true;
        visitedBackup[start] = true;

        cout << "BFS: ";

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            cout << current << " ";

            for (int i = 0; i < n; i++) {
                if (copyMatrix[current][i] != 0 && !visited[i]) {
                    visited[i] = true;
                    visitedBackup[i] = true;
                    q.push(i);
                }
            }
        }

        cout << endl;
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

    cout << "===== BFS =====" << endl;
    g.BFS(0);

    return 0;
}