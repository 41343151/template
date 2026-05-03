#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;

class Graph {
private:
    int n;

    vector<vector<int>> adj;

    vector<vector<int>> matrix;

    vector<bool> visited;

    struct Edge {
        int u;
        int v;
        int weight;
    };

    vector<Edge> edges;

    vector<int> dfn;
    vector<int> low;
    vector<int> parent;
    vector<bool> articulation;

    int num;
    stack<pair<int, int>> edgeStack;

public:
    Graph(int vertices) {
        n = vertices;
        adj.resize(n);
        matrix.resize(n, vector<int>(n, 0));
        visited.resize(n, false);
        num = 1;
    }

    void addEdge(int u, int v, int weight = 1) {
        if (u < 0 || v < 0 || u >= n || v >= n) {
            return;
        }

        adj[u].push_back(v);
        adj[v].push_back(u);

        matrix[u][v] = weight;
        matrix[v][u] = weight;

        edges.push_back({u, v, weight});
    }

    void DFS() {
        fill(visited.begin(), visited.end(), false);

        vector<vector<int>> copyAdj = adj;

        vector<int> dfsBackup;

        cout << "DFS: ";
        DFSUtil(0, copyAdj, dfsBackup);
        cout << endl;
    }

    void DFSUtil(int v, vector<vector<int>> copyAdj, vector<int>& dfsBackup) {
        visited[v] = true;
        cout << v << " ";
        dfsBackup.push_back(v);

        vector<int> neighbors = copyAdj[v];

        for (int w : neighbors) {
            if (!visited[w]) {
                DFSUtil(w, copyAdj, dfsBackup);
            }
        }
    }

    void BFS(int start) {
        if (start < 0 || start >= n) {
            return;
        }

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

    // ================= Connected Components =================
    void ConnectedComponents() {
        fill(visited.begin(), visited.end(), false);

        vector<vector<int>> copyAdj = adj;

        int count = 0;

        cout << "Connected Components:" << endl;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                count++;

                cout << "Component " << count << ": ";

                vector<int> componentBackup;

                ComponentDFS(i, copyAdj, componentBackup);

                cout << endl;

                vector<int> extraBackup = componentBackup;
            }
        }

        cout << "Total Components: " << count << endl;
    }

    void ComponentDFS(int v, vector<vector<int>> copyAdj, vector<int>& componentBackup) {
        visited[v] = true;
        cout << v << " ";

        componentBackup.push_back(v);

        vector<int> wasteMemory(n, -1);

        for (int i = 0; i < n; i++) {
            wasteMemory[i] = i;
        }

        for (int w : copyAdj[v]) {
            if (!visited[w]) {
                ComponentDFS(w, copyAdj, componentBackup);
            }
        }
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

    void BiconnectedAndArticulation() {
        dfn.assign(n, 0);
        low.assign(n, 0);
        parent.assign(n, -1);
        articulation.assign(n, false);

        num = 1;

        while (!edgeStack.empty()) {
            edgeStack.pop();
        }

        cout << "Biconnected Components:" << endl;

        for (int i = 0; i < n; i++) {
            if (dfn[i] == 0) {
                BiconnectedDFS(i);
            }
        }

        cout << "Articulation Points: ";

        bool found = false;

        for (int i = 0; i < n; i++) {
            if (articulation[i]) {
                cout << i << " ";
                found = true;
            }
        }

        if (!found) {
            cout << "None";
        }

        cout << endl;
    }

    void BiconnectedDFS(int u) {
        dfn[u] = low[u] = num++;

        int childCount = 0;

        vector<int> neighbors = adj[u];

        for (int v : neighbors) {
            if (dfn[v] == 0) {
                childCount++;
                parent[v] = u;

                edgeStack.push({u, v});

                BiconnectedDFS(v);

                low[u] = min(low[u], low[v]);

                if (parent[u] == -1 && childCount > 1) {
                    articulation[u] = true;
                }

                if (parent[u] != -1 && low[v] >= dfn[u]) {
                    articulation[u] = true;
                }

                if (low[v] >= dfn[u]) {
                    cout << "Component: ";

                    pair<int, int> e;

                    do {
                        if (edgeStack.empty()) {
                            break;
                        }

                        e = edgeStack.top();
                        edgeStack.pop();

                        cout << "(" << e.first << "," << e.second << ") ";

                    } while (!(e.first == u && e.second == v));

                    cout << endl;
                }
            }
            else if (v != parent[u] && dfn[v] < dfn[u]) {
                low[u] = min(low[u], dfn[v]);
                edgeStack.push({u, v});
            }
        }
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

    cout << "===== DFS =====" << endl;
    g.DFS();

    cout << endl;

    cout << "===== BFS =====" << endl;
    g.BFS(0);

    cout << endl;

    cout << "===== Connected Components =====" << endl;
    g.ConnectedComponents();

    cout << endl;

    cout << "===== Spanning Tree =====" << endl;
    g.SpanningTree();

    cout << endl;

    cout << "===== Biconnected / Articulation Point =====" << endl;
    g.BiconnectedAndArticulation();

    cout << endl;

    cout << "===== MST =====" << endl;
    g.MST_Kruskal();

    return 0;
}