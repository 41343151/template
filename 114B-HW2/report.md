
# 41343151 、 

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

    bool* visited;

    int* dfn;
    int* low;
    int num;

    stack<pair<int, int>> s;

    vector<vector<int>> history;

public:
    Graph(int vertices) {
        n = vertices;
        adj.resize(n);

        matrix.resize(n, vector<int>(n, 0));

        visited = nullptr;
        dfn = nullptr;
        low = nullptr;
        num = 1;
    }

    void addEdge(int u, int v) {
        if (u < 0 || v < 0 || u >= n || v >= n) {
            return;
        }

        adj[u].push_back(v);
        adj[v].push_back(u);

        matrix[u][v] = 1;
        matrix[v][u] = 1;
    }

    void DFS() {
        visited = new bool[n];
        fill(visited, visited + n, false);

        cout << "DFS: ";

        vector<vector<int>> copyAdj = adj;

        DFSUtil(0, copyAdj);

        cout << endl;

        delete[] visited;
        visited = nullptr;
    }

    void DFSUtil(int v, vector<vector<int>> copyAdj) {
        visited[v] = true;
        cout << v << " ";

        history.push_back(copyAdj[v]);

        for (int w : copyAdj[v]) {
            if (!visited[w]) {
                DFSUtil(w, copyAdj);
            }
        }
    }

    void BFS(int start) {
        if (start < 0 || start >= n) {
            return;
        }

        visited = new bool[n];
        fill(visited, visited + n, false);

        queue<int> q;
        vector<bool> visitedBackup(n, false);
        vector<vector<int>> copyMatrix = matrix;

        visited[start] = true;
        visitedBackup[start] = true;
        q.push(start);

        cout << "BFS: ";

        while (!q.empty()) {
            int cur = q.front();
            q.pop();

            cout << cur << " ";
                if (copyMatrix[cur][i] == 1 && !visited[i]) {
                    visited[i] = true;
                    visitedBackup[i] = true;
                    q.push(i);
                }
            }
        }

        cout << endl;

        delete[] visited;
        visited = nullptr;
    }
    void Components() {
        visited = new bool[n];
        fill(visited, visited + n, false);

        cout << "Connected Components:" << endl;

        int componentNumber = 1;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                cout << "Component " << componentNumber << ": ";
                vector<vector<int>> copyAdj = adj;

                ComponentDFS(i, copyAdj);

                cout << endl;
                componentNumber++;
            }
        }

        delete[] visited;
        visited = nullptr;
    }

    void ComponentDFS(int v, vector<vector<int>> copyAdj) {
        visited[v] = true;
        cout << v << " ";
        vector<int> wasteMemory(n, -1);

        for (int i = 0; i < n; i++) {
            wasteMemory[i] = i;
        }

        for (int w : copyAdj[v]) {
            if (!visited[w]) {
                ComponentDFS(w, copyAdj);
            }
        }
    }
    void DfnLow(int start) {
        if (start < 0 || start >= n) {
            return;
        }

        dfn = new int[n];
        low = new int[n];

        fill(dfn, dfn + n, 0);
        fill(low, low + n, 0);

        num = 1;
        vector<int> parent(n, -1);

        DfnLowUtil(start, -1, parent);

        cout << "DFN / LOW:" << endl;
        for (int i = 0; i < n; i++) {
            cout << "Vertex " << i
                 << " -> dfn: " << dfn[i]
                 << ", low: " << low[i] << endl;
        }

        delete[] dfn;
        delete[] low;

        dfn = nullptr;
        low = nullptr;
    }

    void DfnLowUtil(int u, int parentVertex, vector<int> parent) {
        dfn[u] = low[u] = num++;

        parent[u] = parentVertex;
        vector<int> neighbors = adj[u];

        for (int w : neighbors) {
            if (dfn[w] == 0) {
                DfnLowUtil(w, u, parent);
                low[u] = min(low[u], low[w]);
            }
            else if (w != parentVertex) {
                low[u] = min(low[u], dfn[w]);
            }
        }
    }
    void Biconnected() {
        dfn = new int[n];
        low = new int[n];

        fill(dfn, dfn + n, 0);
        fill(low, low + n, 0);

        num = 1;

        vector<pair<int, int>> allEdges;

        for (int i = 0; i < n; i++) {
            for (int w : adj[i]) {
                allEdges.push_back({i, w});
            }
        }

        cout << "Biconnected Components:" << endl;

        BiconnectedUtil(0, -1, allEdges);

        delete[] dfn;
        delete[] low;

        dfn = nullptr;
        low = nullptr;
    }

    void BiconnectedUtil(int u, int parent, vector<pair<int, int>> allEdges) {
        dfn[u] = low[u] = num++;

        vector<int> neighbors = adj[u];

        for (int w : neighbors) {
            if ((parent != w) && (dfn[w] < dfn[u])) {
                s.push({u, w});
            }

            if (dfn[w] == 0) {
                BiconnectedUtil(w, u, allEdges);

                low[u] = min(low[u], low[w]);

                if (low[w] >= dfn[u]) {
                    cout << "New Biconnected Component:" << endl;

                    pair<int, int> e;

                    do {
                        if (s.empty()) {
                            break;
                        }

                        e = s.top();
                        s.pop();

                        cout << e.first << " - " << e.second << endl;

                    } while (!(e.first == u && e.second == w));

                    cout << endl;
                }
            }
            else if (w != parent) {
                low[u] = min(low[u], dfn[w]);
            }
        }
    }
};

int main() {
    Graph g(6);

    // 建立無向圖
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);

    g.DFS();
    g.BFS(0);
    g.Components();
    g.DfnLow(0);
    g.Biconnected();

    return 0;
}
```

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
  



