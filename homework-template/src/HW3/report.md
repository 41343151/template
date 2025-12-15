
# 41343151

作業三
## 解題說明

本題要求設計一個使用循環連結串列來時做一個C++的Polynomial類別

### 解題策略

1. 定義 Term 儲存係數與指數，ChainNode 用動態資料結構來儲存項數不固定的多項式，Polynomial 用來簡化多項式的操作流程，避免特殊情況。
2. ChainNode<Term>* head 是為了避免處理空串列或第一項的特例以及簡化插入、刪除與走訪。
3. static ChainNode<Term>* avail 是為了提升效能而寫出來的，一直使用new/delete會浪費效能。
4. GetNode(const Term& t) 的作用是有舊節點就使用如果沒有那就創造一個新節點，並設定資料為 t。
5. RetNode(ChainNode<Term>* x) 的作用是把不需要的舊節點刪掉。
6. operator >>(輸入) 為了將「外部表示法」轉為內部 linked list，operator <<(輸出) 為了將內部資料結構轉回題目指定格式。
7. operator + 完成多項式的加法運算，operator - 用於多項式的減法運算，operator * 用於多項式的乘法運算，Evaluate 驗證多項式內容是否正確，並提供實際應用功能。


### 程式實作

以下為主要程式碼：

```cpp


#include <iostream>
#include <cmath>
using namespace std;

//儲存係數與指數//
struct Term {
    int coef;
    int exp;
};

//動態資料結構來儲存項數不固定的多項式//
template <class T>
class ChainNode {
public:
    T data;
    ChainNode<T>* link;

    ChainNode(const T& d = T(), ChainNode<T>* l = nullptr)
        : data(d), link(l) {}
};

//簡化多項式的操作流程//
class Polynomial {
private:
    ChainNode<Term>* head;                  // header node
    static ChainNode<Term>* avail;          // 提升效能

    ChainNode<Term>* GetNode(const Term& t); //creative new node or seek old node and save file is t
    void RetNode(ChainNode<Term>* x);        //delete old node(無用的)

public:
    Polynomial();                           // constructor
    Polynomial(const Polynomial& a);        // copy constructor
    ~Polynomial();                          // destructor

    Polynomial& operator=(const Polynomial& a);

    Polynomial operator+(const Polynomial& b) const;
    Polynomial operator-(const Polynomial& b) const;
    Polynomial operator*(const Polynomial& b) const;

    float Evaluate(float x) const;

    friend istream& operator>>(istream& is, Polynomial& x);
    friend ostream& operator<<(ostream& os, const Polynomial& x);
};


ChainNode<Term>* Polynomial::avail = nullptr;


ChainNode<Term>* Polynomial::GetNode(const Term& t) {
    if (avail) {
        ChainNode<Term>* x = avail;
        avail = avail->link;
        x->data = t;
        return x;
    }
    return new ChainNode<Term>(t);
}

void Polynomial::RetNode(ChainNode<Term>* x) {
    x->link = avail;
    avail = x;
}


Polynomial::Polynomial() {
    head = new ChainNode<Term>();
    head->link = head;   // circular
}

Polynomial::~Polynomial() {
    ChainNode<Term>* cur = head->link;
    while (cur != head) {
        ChainNode<Term>* temp = cur;
        cur = cur->link;
        RetNode(temp);
    }
    delete head;
}


Polynomial::Polynomial(const Polynomial& a) : Polynomial() {
    *this = a;
}

Polynomial& Polynomial::operator=(const Polynomial& a) {
    if (this == &a) return *this;

    this->~Polynomial();

    head = new ChainNode<Term>();
    head->link = head;

    ChainNode<Term>* rear = head;
    ChainNode<Term>* cur = a.head->link;

    while (cur != a.head) {
        rear->link = GetNode(cur->data);
        rear = rear->link;
        cur = cur->link;
    }
    rear->link = head;
    return *this;
}


istream& operator>>(istream& is, Polynomial& x) { //cin change
    int n;
    is >> n;

    ChainNode<Term>* rear = x.head;
    for (int i = 0; i < n; i++) {
        Term t;
        is >> t.coef >> t.exp;
        rear->link = x.GetNode(t);
        rear = rear->link;
    }
    rear->link = x.head;
    return is;
}


ostream& operator<<(ostream& os, const Polynomial& x) { //cout change
    int count = 0;
    ChainNode<Term>* cur = x.head->link;

    while (cur != x.head) {
        count++;
        cur = cur->link;
    }

    os << count;
    cur = x.head->link;
    while (cur != x.head) {
        os << " " << cur->data.coef << " " << cur->data.exp;
        cur = cur->link;
    }
    return os;
}

//加法//
Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial c;
    ChainNode<Term>* aPtr = head->link;
    ChainNode<Term>* bPtr = b.head->link;
    ChainNode<Term>* rear = c.head;

    while (aPtr != head && bPtr != b.head) {
        if (aPtr->data.exp == bPtr->data.exp) {
            int sum = aPtr->data.coef + bPtr->data.coef;
            if (sum != 0) {
                rear->link = c.GetNode({sum, aPtr->data.exp});
                rear = rear->link;
            }
            aPtr = aPtr->link;
            bPtr = bPtr->link;
        }
        else if (aPtr->data.exp > bPtr->data.exp) {
            rear->link = c.GetNode(aPtr->data);
            rear = rear->link;
            aPtr = aPtr->link;
        }
        else {
            rear->link = c.GetNode(bPtr->data);
            rear = rear->link;
            bPtr = bPtr->link;
        }
    }

    while (aPtr != head) {
        rear->link = c.GetNode(aPtr->data);
        rear = rear->link;
        aPtr = aPtr->link;
    }

    while (bPtr != b.head) {
        rear->link = c.GetNode(bPtr->data);
        rear = rear->link;
        bPtr = bPtr->link;
    }

    rear->link = c.head;
    return c;
}

//減法//
Polynomial Polynomial::operator-(const Polynomial& b) const {
    Polynomial negB = b;
    for (auto cur = negB.head->link; cur != negB.head; cur = cur->link) {
        cur->data.coef = -cur->data.coef;
    }
    return (*this + negB);
}

//乘法//
Polynomial Polynomial::operator*(const Polynomial& b) const {
    Polynomial c;

    for (auto aPtr = head->link; aPtr != head; aPtr = aPtr->link) {
        Polynomial temp;
        ChainNode<Term>* rear = temp.head;

        for (auto bPtr = b.head->link; bPtr != b.head; bPtr = bPtr->link) {
            Term t;
            t.coef = aPtr->data.coef * bPtr->data.coef;
            t.exp  = aPtr->data.exp + bPtr->data.exp;
            rear->link = temp.GetNode(t);
            rear = rear->link;
        }
        rear->link = temp.head;
        c = c + temp;
    }
    return c;
}

//帶入數字進去//
float Polynomial::Evaluate(float x) const {
    float result = 0;
    for (auto cur = head->link; cur != head; cur = cur->link) {
        result += cur->data.coef * pow(x, cur->data.exp);
    }
    return result;
}


int main() {
    Polynomial p1, p2;

    cout << "Input polynomial 1: ";
    cin >> p1;
    cout << "Input polynomial 2: ";
    cin >> p2;

    cout << "P1 = " << p1 << endl;
    cout << "P2 = " << p2 << endl;

    cout << "P1 + P2 = " << (p1 + p2) << endl;
    cout << "P1 - P2 = " << (p1 - p2) << endl;
    cout << "P1 * P2 = " << (p1 * p2) << endl;

    cout << "P1(2) = " << p1.Evaluate(2) << endl;

    return 0;
}

```


## 效能分析

1. 時間複雜度：程式的時間複雜度為 *Eval:O(t) Add:O(n+m) Mult:O((nm)^2)*。
2. 空間複雜度：空間複雜度為 *Eval:O(1) Add:O(n+m) Mult:O((nm))*。

## 測試與驗證

### 測試案例

<img width="392" height="156" alt="image" src="https://github.com/user-attachments/assets/a444156d-fe6d-4ae9-b4b7-18cc2f015b3b" />

<img width="595" height="156" alt="image" src="https://github.com/user-attachments/assets/c40ff859-b5e0-459e-9803-2ebb83c43db9" />

<img width="432" height="152" alt="image" src="https://github.com/user-attachments/assets/d5562730-7d0b-4ba9-88d1-d3571196fa34" />



### 編譯與執行指令

```shell
$ g++ -std=c++17 -O2 -Wall Homework2.cpp -o hw2.exe
$ .\hw2.exe=
```

### 結論

1. 程式能正確計算當*exp*跟*coef*個別是多少時的答案。  
2. 在*exp*是*0*或*負數*的情況下，程式是否正確，符合設計預期。  

## 心得討論

##為什麼使用 *operator>>*與*operator<<*
*operator>>*：讓「輸入」多項式像基本型別一樣直覺（cin >> p;）。同時能把「讀 n、讀 (exp coef) 組」的規格集中管理，避免主程式散落解析邏輯。
*operator<<*：把「數學友善輸出」集中處理（省略 1*、處理正負號、x^1 不印 ^1），主程式只要 cout << p 就能漂亮輸出，維護性高。

### 選擇*friend*的原因

在本程式中，使用friend來寫這串*code*的主要原因如下：

1. **允許內部直接訪問私有資料**
   在 Polynomial 的成員函式裡（例如 Eval()、normalize()）可以直接寫：  
   s += termArray[i].coef * pow(x, termArray[i].exp); 
   這是可行的，因為 Term 對 Polynomial 是 friend;沒有這行 friend 的話則會出現error。

2. **好處**  
   Polynomial 是唯一能操作 Term 內部的類別。
   外部（例如 main()）不能直接動 coef 或 exp，防止亂改資料。
   
3. **沒有使用friend的情況**  
   因Term的私有資料無法被Polynomial使用，所以coef和exp會是pritave。
   程式碼會變得更長一串、更消耗效能、記憶體之類的。
   因為少了friend就代表不能直接存取，需要跑好幾個流程才可以存取。
   所以程式執行的時間一旦長起來，消耗的東西就會增長。


### 心得
在上學期就有學習過C++中的*friend*，但在課堂實際使用卻很少，這次的功課難得出現可以使用*friend*的程式，因為這次的程式碼很長一串，我當時就在講要怎麼把效能提升上去，不要吃這麼多記憶體，突然就想到了之前學過的*friend*而且剛好，這段程式碼又很常去抓函式裡面的數值或變數。
這次使用*friend*讓我對這個程式有更深入的理解，像是*friend*常使用的方式: *friend class X*
 *friend 函式宣告*，而且我覺得*friend*這個程式很有趣，跟他的名字一樣，是朋友，朋友之間可以互相存取，但朋友的朋友不能存取，很像是雙方的小祕密一樣。
 這次使用*friend*讓我知道他的優缺點，優點是可以緊密合作、保持封裝、參數不容易被修改、被保護得很好。缺點是當濫用了*friend*會讓類別之間相似度過高，很容易改了一邊參數另外一邊也一起更動了，可維護性和測試性會下降，因為太多朋友了會導致邊界模糊。
