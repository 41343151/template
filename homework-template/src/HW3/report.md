
# 41343151

作業三
## 解題說明

本題要求設計一個使用循環連結串列來時做一個C++的Polynomial類別

### 解題策略

1. 定義 Term 儲存係數與指數，Polynomial 用動態陣列管理多項式項目。  
2. 以 operator+、operator* 完成多項式加法與乘法，用 Eval() 計算多項式值。
3. *Add()*是把兩個多項式 p(x) 和 q(x) 相加，得到新多項式 r(x) = p(x) + q(x)，邏輯上是「指數相同的項合併、指數不同的直接保留」。
4. *Mult()*將兩個多項式 p(x) 和 q(x) 相乘。
5. *Eval()*是將變數 x 代入多項式 p(x) 中，計算其數值結果。

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

class Polynomial; // 前置宣告，讓 Term 可宣告 friend

// ---- Term：一個非零項 (coef * x^exp) ----
class Term {
    friend class Polynomial;           // 讓 Polynomial 可直接訪問 coef/exp
private:
    float coef;                        // 係數
    int   exp;                         // 指數（建議維持為非負整數）
    Term(float c = 0.0f, int e = 0) : coef(c), exp(e) {}
public:
    // 提供唯讀 getters（友善排序/比較/輸出流程）
    float getCoef() const { return coef; }
    int   getExp()  const { return exp;  }
};

// ---- Polynomial：以動態陣列存非零項（不保證稀疏度，但只存非零）----
class Polynomial {
public:
    // 建構子：capacity 至少為 1；terms=0 表示目前沒有項
    explicit Polynomial(int cap = 4) : capacity(std::max(1, cap)), terms(0) {
        termArray = new Term[capacity];
    }

    // 拷貝建構：深拷貝 termArray
    Polynomial(const Polynomial& rhs) : capacity(rhs.capacity), terms(rhs.terms) {
        termArray = new Term[capacity];
        std::copy(rhs.termArray, rhs.termArray + terms, termArray);
