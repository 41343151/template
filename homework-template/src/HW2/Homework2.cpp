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
    float coef; // �Y��
    int   exp;  // ����
    Term(float c = 0.0f, int e = 0) : coef(c), exp(e) {}
public:
    float getCoef() const { return coef; }
    int   getExp()  const { return exp;  }
};

class Polynomial {
public:
    // �غc/�Ѻc/����/���w
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

    // �]�w��@���]�۰ʦX�֦P exp�^
    void setTerm(int exp, float coef) { addInPlace(exp, coef); normalize(); }

    // �D��
    float Eval(float x) const {
        float s = 0.0f;
        for (int i = 0; i < terms; ++i)
            s += termArray[i].coef * std::pow(x, termArray[i].exp);
        return s;
    }

    // �D�ح쫬�G���Ѧ��� Add/Mult�A������I�s�B��l
    Polynomial Add(const Polynomial& p)  const { return *this + p; }
    Polynomial Mult(const Polynomial& p) const { return *this * p; }

    // ---- �ͤ��B��l�]�D�����^----
    friend istream& operator>>(istream& is, Polynomial& p);
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    friend Polynomial operator+(const Polynomial& a, const Polynomial& b);
    friend Polynomial operator*(const Polynomial& a, const Polynomial& b);

private:
    // �������
    Term* termArray; // �D�s���}�C
    int   capacity;  // �}�C�e�q
    int   terms;     // �w�ζ���

    // �p�u��
    void ensureCapacity(int need) {
        if (need <= capacity) return;
        int nc = std::max(need, capacity * 2);
        Term* na = new Term[nc];
        std::copy(termArray, termArray + terms, na);
        delete[] termArray;
        termArray = na;
        capacity  = nc;
    }
    void append(int exp, float coef) { // �����X�֡B�����Ƨ�
        if (std::fabs(coef) == 0.0f) return;
        ensureCapacity(terms + 1);
        termArray[terms] = Term(coef, exp);
        ++terms;
    }
    void addInPlace(int exp, float coef) { // ��J�� exp �֥[
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
    void normalize() { // �� exp �j��p�ƧǡB�X�֡B�h 0
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

// ---- friend operators ���w�q ----

// ��J�榡�G�� n�A���� n �� (exp coef)
// �Ҧp�G3  4 3  2 2  0 1  �N�� 3x^4 + 2x^2 + 1
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
    p = tmp; // �ϥΫ������w
    return is;
}

// �H�ƾǤ͵����榡��X�]�ٲ� 1*x�B�B�z �ӡ^
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
    cout << "�п�J�h���� p �����ƻP (exp coef)�G\n";
    Polynomial p; cin >> p;
    cout << "�п�J�h���� q �����ƻP (exp coef)�G\n";
    Polynomial q; cin >> q;

    cout << "p(x) = " << p << "\n";
    cout << "q(x) = " << q << "\n";

    cout << "p(x) + q(x) = " << (p + q) << "\n";
    cout << "p(x) * q(x) = " << (p * q) << "\n";
    cout << "p(2) = " << p.Eval(2.0f) << "\n";
    return 0;
}
