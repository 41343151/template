
# 41343151

作業三
## 解題說明

本題要求設計一個使用循環連結串列來時做一個C++的Polynomial類別

### 解題策略

1. 定義 Term 儲存係數與指數，ChainNode 用動態資料結構來儲存項數不固定的多項式，Polynomial 用來簡化多項式的操作流程，避免特殊情況。\\
2. ChainNode<Term>* head 是為了避免處理空串列或第一項的特例以及簡化插入、刪除與走訪。
3. static ChainNode<Term>* avail 是為了提升效能
4. operator >> 將題目指定的外部表示法轉換成內部 linked list ，operator >> 將內部 linked list 還原成題目要求的輸出格式。
5. Copy Constructor & Assignment Operator 用來確保多項式之間是「獨立存在」，避免共用節點。
6. Destructor 正確釋放多項式所使用的動態記憶體。
7. operator + 完成多項式的加法運算，operator - 用於多項式的減法運算，operator * 用於多項式的乘法運算，Evaluate 驗證多項式內容是否正確，並提供實際應用功能。

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
