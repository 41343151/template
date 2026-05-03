#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Graph {
private:
    int n;
    vector<vector<int>> adj;
    vector<vector<int>> matrix;

    struct Edge {
        int u;
        int v;
        int weight;
    };

    vector<Edge> edges;

public:
    Graph(int vertices) {
        n = vertices;
        adj.resize(n);
        matrix.resize(n, vector<int>(n, 0));
    }

    void addEdge(int u, int v, int weight = 1) {
        adj[u].push_back(v);
        adj[v].push_back(u);

        matrix[u][v] = weight;
        matrix[v][u] = weight;

        edges.push_back({u, v, weight});
    }

    int findParent(vector<int>& parentSet, int x) {
        if (parentSet[x] == x) {
            return x;
        }

        return parentSet[x] = findParent(parentSet, parentSet[x]);
    }

    void unionSet(vector<int>& parentSet, vector<int>& rankSet, int a, int b) {
        int rootA = findParent(parentSet, a);
        int rootB = findParent(parentSet, b);

        if (rootA != rootB) {
            if (rankSet[rootA] < rankSet[rootB]) {
                parentSet[rootA] = rootB;
            }
            else if (rankSet[rootA] > rankSet[rootB]) {
                parentSet[rootB] = rootA;
            }
            else {
                parentSet[rootB] = rootA;
                rankSet[rootA]++;
            }
        }
    }

    void MST_Kruskal() {
        vector<Edge> copyEdges = edges;
        vector<vector<int>> matrixBackup = matrix;

        sort(copyEdges.begin(), copyEdges.end(), [](Edge a, Edge b) {
            return a.weight < b.weight;
        });

        vector<int> parentSet(n);
        vector<int> rankSet(n, 0);

        for (int i = 0; i < n; i++) {
            parentSet[i] = i;
        }

        vector<Edge> mstEdges;
        int totalCost = 0;

        for (Edge e : copyEdges) {
            int rootU = findParent(parentSet, e.u);
            int rootV = findParent(parentSet, e.v);

            if (rootU != rootV) {
                mstEdges.push_back(e);
                totalCost += e.weight;
                unionSet(parentSet, rankSet, e.u, e.v);
            }
        }

        cout << "MST Edges:" << endl;

        for (Edge e : mstEdges) {
            cout << e.u << " - " << e.v
                 << " weight = " << e.weight << endl;
        }

        cout << "Total MST Cost: " << totalCost << endl;

        vector<Edge> backupMST = mstEdges;
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

    cout << "===== MST =====" << endl;
    g.MST_Kruskal();

    return 0;
}