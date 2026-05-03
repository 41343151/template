
# 41343151 、 41343117

作業二
## 解題說明

本題要求參考題目附圖MaxPQ範例，寫出相似的C++抽象列別並實作MinPQ。

### 解題策略

1. 
2. 
3. 
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

        // 瑕疵：同時用 matrix 儲存
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
1. 時間複雜度：程式的時間複雜度為 *O(n)*。
2. 空間複雜度：空間複雜度為 *O(n)*。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數          | 預期輸出 | 實際輸出 |
|----------|-----------------|----------|----------|
| 測試一   | *10,4,15,2,8,1* | 1  2  4  | 1  2  4  |
| 測試二   | *9,3,-5,12,7,2* | -5  2  3  | -5  2  3  |
| 測試三   | *23,7,0,12,13,2*| 0  2  7  | 0  2  7  |

### 編譯與執行指令

```shell
g++ graph_homework.cpp -o graph_homework
./graph_homework
```

### 結論

1.     
2. 

### 為甚麼用*heap*來解題

1. **程式邏輯簡單直觀**  
   插入:新元素放最後，再往上調整，很像FIFO的概念，但在最後會去做排序。
   刪除最小值:把最後元素補到根，再往下調整。
   這樣才能維持 Top 很快 和 Push Pop 都是 O(log n)。

### 心得與討論
這次的作業很有趣，利用優先佇列來做出類似二元述概念，可以讓我在學習二元樹的時候對這種程式和這種方式更加深度理解和印象。
在題目的附圖讓我知道MaxPQ和MinPQ的概念一模一樣，只是差在一個是取最大值和刪除最大值，另一個則是取最小值和刪除最小值，在這次的作業中不僅讓我學習最小堆積與最小優先權隊列，還讓我練習了一直都很不熟悉的抽象類別，雖然知道抽象類別的概念，但每次實作都是完全一頭霧水、完全搞不懂，這次以最小堆積與最小優先權隊列讓我了解二元樹的概念，就有點像是能源效率分級標示，但跟這個不同的是，二元樹只會有元素，也就是數值。這次功課中的Min Heap(最小堆積)是一種完全二元樹的表現方式，父節點的值永遠小於或等於子節點，讓我對二元樹有不同的理解。
  



