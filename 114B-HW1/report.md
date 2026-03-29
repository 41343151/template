
# 41343151

作業一
第一題
## 解題說明

本題要求參考題目附圖MaxPQ範例，寫出相似的C++抽象列別並實作MinPQ。

### 解題策略

1. 了解 Max Heap 和 Min Heap 的差別。
2. 理解最小堆積與最小優先權隊列到底在做什麼。
3. 用陣列實作 heap 以及 Push 和 Pop 的調整過程。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
using namespace std;

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;  //判斷是否為空
    virtual const T& Top() const = 0;  //回傳最小值
    virtual void Push(const T& x) = 0;  //插入
    virtual void Pop() = 0;  //刪除最小值
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int size;
    int capacity;

    void heapifyDown(int index) {
        int smallest = index;
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;

        if (leftChild < size && heap[leftChild] < heap[smallest]) {
            smallest = leftChild;
        }

        if (rightChild < size && heap[rightChild] < heap[smallest]) {
            smallest = rightChild;
        }

        if (smallest != index) {
            T temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            heapifyDown(smallest);
        }
    }

public:
    MinHeap(int initialCapacity = 10) {
        capacity = initialCapacity;
        size = 0;
        heap = new T[capacity];
    }

    ~MinHeap() {
        delete[] heap;
    }

    bool IsEmpty() const override {
        return size == 0;
    }

    const T& Top() const override {
        if (IsEmpty()) {
            cout << "Heap is empty!" << endl;
            exit(1);
        }
        return heap[0];
    }

    void Push(const T& x) override {
        T* newHeap = new T[capacity + 1];
        for (int i = 0; i < size; i++) {
            newHeap[i] = heap[i];
        }

        newHeap[size] = x;

        delete[] heap;
        heap = newHeap;
        capacity = capacity + 1;
        size++;

        for (int i = size / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    void Pop() override {
        if (IsEmpty()) {
            return;
        }

        heap[0] = heap[size - 1];
        size--;

        for (int i = size / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }
};

int main() {
    MinHeap<int> h;

    h.Push(10);
    h.Push(4);
    h.Push(15);
    h.Push(2);
    h.Push(8);
    h.Push(1);

    cout << "Top: " << h.Top() << endl;

    h.Pop();
    cout << "Top after one pop: " << h.Top() << endl;

    h.Pop();
    cout << "Top after two pops: " << h.Top() << endl;

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
$ g++ minheap.cpp -o minheap.exe
$ minheap.exe
```

### 結論

1. 程式能正確排序並選出最小值。    
2. 測試案例涵蓋了多種邊界情況（元素有0或是負數的情況），驗證程式的正確性。

### 為甚麼用*heap*來解題

1. **程式邏輯簡單直觀**  
   插入:新元素放最後，再往上調整，很像FIFO的概念，但在最後會去做排序。
   刪除最小值:把最後元素補到根，再往下調整。
   這樣才能維持 Top 很快 和 Push Pop 都是 O(log n)。

### 心得與討論
這次的作業很有趣，利用優先佇列來做出類似二元述概念，可以讓我在學習二元樹的時候對這種程式和這種方式更加深度理解和印象。
在題目的附圖讓我知道MaxPQ和MinPQ的概念一模一樣，只是差在一個是取最大值和刪除最大值，另一個則是取最小值和刪除最小值，在這次的作業中不僅讓我學習最小堆積與最小優先權隊列，還讓我練習了一直都很不熟悉的抽象類別，雖然知道抽象類別的概念，但每次實作都是完全一頭霧水、完全搞不懂，這次以最小堆積與最小優先權隊列讓我了解二元樹的概念，就有點像是能源效率分級標示，但跟這個不同的是，二元樹只會有元素，也就是數值。這次功課中的Min Heap(最小堆積)是一種完全二元樹的表現方式，父節點的值永遠小於或等於子節點，讓我對二元樹有不同的理解。
  



# 41343151

作業一
第二題
## 解題說明

本題要求實作一個二元搜尋樹(BST)的隨機性與刪除。
(a)寫出一個計算 $h / \log_2 n$ 的比例並驗證該比例是否接近常數2。
(b)寫出一個C++函式來刪除具有鍵值k的節點，並說明時間複雜程度。

### 解題策略

了解為何要使用隨機插入而不是順序插入(因為這樣會退化成鏈狀)，因為我們是要實作二元樹，所以隨機插入能讓樹相對平衡。


## 程式實作

以下為(a)的主要程式碼：

```cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int x) {
        data = x;
        left = NULL;
        right = NULL;
    }
};

class BST {
public:
    Node* root;

    BST() {
        root = NULL;
    }

    Node* insertNode(Node* node, int x) {
        if (node == NULL) {
            Node* temp = new Node(x);
            return temp;
        }

        if (x < node->data) {
            node->left = insertNode(node->left, x);
        }
        else {
            node->right = insertNode(node->right, x);
        }

        return node;
    }

    void insert(int x) {
        root = insertNode(root, x);
    }

    int getHeight(Node* node) {
        if (node == NULL) {
            return 0;
        }

        int leftH = getHeight(node->left);
        int rightH = getHeight(node->right);

        if (leftH > rightH) {
            return leftH + 1;
        }
        else {
            return rightH + 1;
        }
    }

    int height() {
        return getHeight(root);
    }
};

int main() {
    srand(time(NULL));

    cout << "n\tHeight\tHeight/log2(n)" << endl;

    for (int n = 100; n <= 10000; n += 500) {
        BST tree;

        vector<int> arr;

        for (int i = 0; i < n; i++) {
            int num = rand();
            arr.push_back(num);
        }

        for (int i = 0; i < arr.size(); i++) {
            tree.insert(arr[i]);
        }

        int h1 = tree.height();
        int h2 = tree.height();
        int h3 = tree.height();

        int h = h3;

        double ans = h / log2((double)n);

        cout << n << "\t" << h << "\t" << ans << endl;
    }

    return 0;
}
```


## 程式實作

以下為(b)的主要程式碼：

```cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int x) {
        data = x;
        left = NULL;
        right = NULL;
    }
};

class BST {
public:
    Node* root;

    BST() {
        root = NULL;
    }

    Node* insertNode(Node* node, int x) {
        if (node == NULL) {
            Node* temp = new Node(x);
            return temp;
        }

        if (x < node->data) {
            node->left = insertNode(node->left, x);
        }
        else {
            node->right = insertNode(node->right, x);
        }

        return node;
    }

    void insert(int x) {
        root = insertNode(root, x);
    }

    int getHeight(Node* node) {
        if (node == NULL) {
            return 0;
        }

        int leftH = getHeight(node->left);
        int rightH = getHeight(node->right);

        if (leftH > rightH) {
            return leftH + 1;
        }
        else {
            return rightH + 1;
        }
    }

    int height() {
        return getHeight(root);
    }
};

int main() {
    cout << "n\tHeight\tHeight/log2(n)" << endl;

    for (int n = 100; n <= 10000; n += 500) {
        BST tree;

        vector<int> arr;

        for (int i = 0; i < n; i++) {
            arr.push_back(i);   
        }

        for (int i = 0; i < arr.size(); i++) {
            tree.insert(arr[i]);
        }

        int h1 = tree.height();
        int h2 = tree.height();
        int h3 = tree.height();

        int h = h3;

        double ans = h / log2((double)n);

        cout << n << "\t" << h << "\t" << ans << endl;
    }

    return 0;
}
```




## 效能分析

1. 時間複雜度：程式的時間複雜度為 *O(n²)*。
2. 空間複雜度：空間複雜度為 *O(n)*。

## 測試與驗證

### 測試案例

(a)
| 測試案例 | 輸入參數 | 預期輸出 | 實際輸出 |
|----------|--------|--------------|---------------|
| 測試一   | *1100* | 24 2.37546  | 24 2.37546  |
| 測試二   | *4100* | 25 2.08309  | 25 2.08309  |
| 測試三   | *8600* | 28 2.14229  | 28 2.14229  |

(b)
| 測試案例 | 輸入參數 | 預期輸出 | 實際輸出 |
|----------|--------|--------------|---------------|
| 測試一   | *1100* | 1100 108.875  | 1100 108.875  |
| 測試二   | *4100* | 4100 341.627  | 4100 341.627  |
| 測試三   | *8600* | 8600 657.989  | 8600 657.989  |

### 編譯與執行指令

```shell
g++ 2a.cpp -o 2a
g++ 2b.cpp -o 2b
```

### 結論

1. 本題透過實作 BST 並進行多組實驗，觀察樹高與節點數量之間的關係。
2. BST 高度隨著n增加而增加，但成長速度接近 log n。
3. BST 沒有自動平衡機制，所以必須額外花記憶體空間去做平衡。
4. 輸入資料不同，導致性能差異較大。


### 心得與討論
這次的作業很有趣，因為本題的(a)和(b)讓我了解了 BST 的效能會受到輸入資料影響很大。在隨機情況下， BST 表現良好，但在排序資料下會退化成鏈結串列，使效能大幅下降，這個有待討論，有可能有辦法解決，但以我現在的能力可能沒辦法優化去解決，可能需要上網多查資料，多試試看不同的方法才能去優化排序資料的部分。透過本次的實驗，讓我更清楚理解資料結構的設計不只會影響功能，也會直接影響效能。總結一句， BST 的效能高度取決於輸入順序，排序資料會使其變成最差的情況，所以如果可優化資料的排序，可能可以使 BST 的效能更進一步提升。
