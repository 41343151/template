
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
在題目的附圖讓我知道MaxPQ和MinPQ的概念一模一樣，只是差在一個是取最大值和刪除最大值，另一個則是取最小值和刪除最小值，在這次的作業中不僅讓我學習最小堆積與最小優先權隊列，還讓我練習了一直都很不熟悉的抽象類別，雖然知道抽象類別的概念，但每次實作都是完全一頭霧水、完全搞不懂，這次以最小堆積與最小優先權隊列讓我了解二元樹的概念，就有點像是能源效率分級標示，但跟這個不同的是，二元樹只會有元素，也就是數值。這次功課中的Min Heap(最小堆積)是一種完全二元樹的表現方式，父節點的值永遠小於或等於子節點，讓我對二元樹有不同的理解，
  
