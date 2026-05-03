
# 41343151 、 41343117

作業二
## 解題說明

本題要求參考PPT做出一個Graph類別，可以存取頂點和邊的資料，再來利用存取的資料去完成DFS、BFS、Connected Components、Spanning Tree、Biconnected / Articulation Pointc和MST最小生成樹。

### 解題策略

1. 利用圖形結構完成多種圖形演算法
2. 建立圖形資料、進行圖形走訪、分析圖的共通性、分析圖形結構
## 程式實作

以下為主要程式碼：

```cpp
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
```

<img width="637" height="760" alt="image" src="https://github.com/user-attachments/assets/d1ab77b8-3811-4622-8d15-d31e39b2c5af" />
CPU只用不到1%
記憶體使用1159.2MB

## 效能分析
1. 時間複雜度：程式的時間複雜度為 *O(V × (V + E) + E log E)*。
2. 空間複雜度：空間複雜度為 *O(V² + E)*。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數V | 輸入參數E | 預期輸出(CC ATP MST) | 實際輸出 |
|---------|-----------|----------|---------------------|-------------|
| 測試一   | *6*      | *6*      | 1 1、3、4 17         | 1 1、3、4 17|
| 測試二   | *7*      | *4*      | 3 1、4 10            | 3 1、4 10  |
| 測試三   | *5*      | *7*      | 1 None 18            | 1 None 18  |

### 編譯與執行指令

```shell
g++ graph_homework.cpp -o graph_homework
./graph_homework
```

### 結論

1.本次作業讓我們練習多種圖形演算法實作 
2.程式使用多種資料結構輔助運算


### 心得與討論
本次作業主要是透過圖形演算法來分析圖的結構與連通關係。程式先建立無向加權圖，再分別使用 DFS、BFS、Connected Components、Spanning Tree、Biconnected / Articulation Point 和 MST 進行處理。透過 DFS 和 BFS 可以了解圖形不同的走訪方式。Connected Components 可以判斷圖中是否有多個連通區塊。Articulation Point 可以找出影響圖形連通性的關鍵頂點。MST 則可以找出連接所有頂點且總成本最低的邊。整體來說，本次作業讓我更了解圖形資料結構不只是單單可以用來表示節點之間的關係，也可以透過不同演算法解決搜尋、連通性判斷與最小成本連接等問題。
  



