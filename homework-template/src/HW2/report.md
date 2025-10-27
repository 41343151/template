
# 41343151

作業二
## 解題說明

本題要求設計一個多項式類別的類別介面與基本運算功能。

### 解題策略

1. 定義 Term 儲存係數與指數，Polynomial 用動態陣列管理多項式項目。  
2. 以 operator+、operator* 完成多項式加法與乘法，用 Eval() 計算多項式值。

## 程式實作

以下為主要程式碼：

```cpp
/*
class Polynomial{
public:
    Polynomial();
    // Construct the polynomial p(x)=0
    Polynomial Add(Polynomial poly);
    // Return the sum of the polynomials this and poly
    Polynomial Mult(Polynomial poly);
    // Return the product of the polynomials this and poly
    float Eval(float f);
    // Evaluate the polynomial this at f and return the result
};
*/

#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

class Polynomial; // forward

class Term {
    friend class Polynomial;
private:
    float coef; // 係數
    int   exp;  // 指數
    Term(float c = 0.0f, int e = 0) : coef(c), exp(e) {}
public:
    float getCoef() const { return coef; }
    int   getExp()  const { return exp;  }
};

class Polynomial {
public:
    // 建構/解構/拷貝/指定
    explicit Polynomial(int cap = 4) : capacity(std::max(1, cap)), terms(0) {
        termArray = new Term[capacity];
    }
    Polynomial(const Polynomial& rhs) : capacity(rhs.capacity), terms(rhs.terms) {
        termArray = new Term[capacity];
        std::copy(rhs.termArray, rhs.termArray + terms, termArray);
    }
    Polynomial& operator=(const Polynomial& rhs) {
        if (this == &rhs) return *this;
        Term* na = new Term[rhs.capacity];
        std::copy(rhs.termArray, rhs.termArray + rhs.terms, na);
        delete[] termArray;
        termArray = na;
        capacity  = rhs.capacity;
        terms     = rhs.terms;
        return *this;
    }
    ~Polynomial() { delete[] termArray; }

    // 設定單一項（自動合併同 exp）
    void setTerm(int exp, float coef) { addInPlace(exp, coef); normalize(); }

    // 求值
    float Eval(float x) const {
        float s = 0.0f;
        for (int i = 0; i < terms; ++i)
            s += termArray[i].coef * std::pow(x, termArray[i].exp);
        return s;
    }

    // 題目原型：提供成員 Add/Mult，內部轉呼叫運算子
    Polynomial Add(const Polynomial& p)  const { return *this + p; }
    Polynomial Mult(const Polynomial& p) const { return *this * p; }

    // ---- 友元運算子（非成員）----
    friend istream& operator>>(istream& is, Polynomial& p);
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    friend Polynomial operator+(const Polynomial& a, const Polynomial& b);
    friend Polynomial operator*(const Polynomial& a, const Polynomial& b);

private:
    // 內部資料
    Term* termArray; // 非零項陣列
    int   capacity;  // 陣列容量
    int   terms;     // 已用項數

    // 小工具
    void ensureCapacity(int need) {
        if (need <= capacity) return;
        int nc = std::max(need, capacity * 2);
        Term* na = new Term[nc];
        std::copy(termArray, termArray + terms, na);
        delete[] termArray;
        termArray = na;
        capacity  = nc;
    }
    void append(int exp, float coef) { // 未必合併、未必排序
        if (std::fabs(coef) == 0.0f) return;
        ensureCapacity(terms + 1);
        termArray[terms] = Term(coef, exp);
        ++terms;
    }
    void addInPlace(int exp, float coef) { // 對既有 exp 累加
        if (std::fabs(coef) == 0.0f) return;
        for (int i = 0; i < terms; ++i) {
            if (termArray[i].exp == exp) {
                termArray[i].coef += coef;
                if (std::fabs(termArray[i].coef) == 0.0f) {
                    for (int k = i + 1; k < terms; ++k) termArray[k - 1] = termArray[k];
                    --terms;
                }
                return;
            }
        }
        append(exp, coef);
    }
    void normalize() { // 依 exp 大到小排序、合併、去 0
        if (terms <= 1) return;
        std::sort(termArray, termArray + terms,
        [](const Term& A, const Term& B){ return A.getExp() > B.getExp(); });
        int w = 0;
        for (int i = 0; i < terms; ) {
            int   e = termArray[i].getExp();
            float c = 0.0f;
            while (i < terms && termArray[i].getExp() == e) c += termArray[i++].getCoef();
            if (std::fabs(c) > 0.0f) {
                termArray[w] = Term(c, e);
                ++w;
            }
        }
        terms = w;
    }
};

// ---- friend operators 的定義 ----

// 輸入格式：先 n，接著 n 組 (exp coef)
// 例如：3  4 3  2 2  0 1  代表 3x^4 + 2x^2 + 1
istream& operator>>(istream& is, Polynomial& p) {
    int n;
    if (!(is >> n)) return is;
    Polynomial tmp(n + 4);
    for (int i = 0; i < n; ++i) {
        int e; float c;
        is >> e >> c;
        tmp.addInPlace(e, c);
    }
    tmp.normalize();
    p = tmp; // 使用拷貝指定
    return is;
}

// 以數學友善的格式輸出（省略 1*x、處理 ±）
ostream& operator<<(ostream& os, const Polynomial& p) {
    if (p.terms == 0) { os << "0"; return os; }
    for (int i = 0; i < p.terms; ++i) {
        float c = p.termArray[i].getCoef();
        int   e = p.termArray[i].getExp();

        if (i > 0) os << (c >= 0 ? " + " : " - ");
        else if (c < 0) os << "-";

        float ac = std::fabs(c);
        bool printCoef = !(ac == 1.0f && e != 0);
        if (printCoef) os << ac;

        if (e > 0) {
            if (printCoef) os << "*";
            os << "x";
            if (e > 1) os << "^" << e;
        }
    }
    return os;
}

// polynomial add
Polynomial operator+(const Polynomial& a, const Polynomial& b) {
    Polynomial r(std::max(a.capacity, b.capacity));
    int i = 0, j = 0;
    while (i < a.terms || j < b.terms) {
        if (j == b.terms || (i < a.terms && a.termArray[i].getExp() > b.termArray[j].getExp())) {
            r.append(a.termArray[i].getExp(), a.termArray[i].getCoef());
            ++i;
        } else if (i == a.terms || b.termArray[j].getExp() > a.termArray[i].getExp()) {
            r.append(b.termArray[j].getExp(), b.termArray[j].getCoef());
            ++j;
        } else {
            float c = a.termArray[i].getCoef() + b.termArray[j].getCoef();
            if (std::fabs(c) > 0.0f) r.append(a.termArray[i].getExp(), c);
            ++i; ++j;
        }
    }
    r.normalize();
    return r;
}

// polynomial mult
Polynomial operator*(const Polynomial& a, const Polynomial& b) {
    Polynomial r(a.terms + b.terms + 4);
    for (int i = 0; i < a.terms; ++i)
        for (int j = 0; j < b.terms; ++j)
            r.addInPlace(a.termArray[i].getExp() + b.termArray[j].getExp(),
                         a.termArray[i].getCoef() * b.termArray[j].getCoef());
    r.normalize();
    return r;
}

int main() {
    cout << "請輸入多項式 p 的項數與 (exp coef)：\n";
    Polynomial p; cin >> p;
    cout << "請輸入多項式 q 的項數與 (exp coef)：\n";
    Polynomial q; cin >> q;

    cout << "p(x) = " << p << "\n";
    cout << "q(x) = " << q << "\n";

    cout << "p(x) + q(x) = " << (p + q) << "\n";
    cout << "p(x) * q(x) = " << (p * q) << "\n";
    cout << "p(2) = " << p.Eval(2.0f) << "\n";
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

