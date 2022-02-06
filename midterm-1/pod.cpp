#include <cassert>
#include <iostream>
using namespace std;

class Weight {
public:
    Weight(int w) : m_w(w) {}
    int getw() const { return m_w; }

private:
    int m_w;
};

class Pod {
public:
    Pod(int p);
    ~Pod();
    Pod(const Pod &other);
    void print() {
        cout << "m_wt: " << m_wt << " -> " << m_wt->getw() << endl
             << "m_ppp: " << m_ppp << endl
             << "m_peas: " << m_peas << " -> " << *m_peas << endl;
    }

private:
    Weight *m_wt;
    int m_ppp;
    int *m_peas;
};

Pod::Pod(int p) {
    m_wt = new Weight(p);
    m_ppp = 3 * p;
    m_peas = &m_ppp;
}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AFTER THE END OF THE DESTRUCTOR AND COPY CONSTRUCTOR
// IMPLEMENTATIONS

Pod::Pod(const Pod &other) : Pod(other.m_wt->getw()) {
}

Pod::~Pod() {
    delete m_wt;
}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT BEFORE THE START OF THE DESTRUCTOR AND COPY CONSTRUCTOR
// IMPLEMENTATIONS

void test() {
    Pod p1(2);
    Pod p2(p1);
    Pod p3(4);
    p1.print();
    p2.print();
    p3.print();
}

int main() {
    test();
    cout << "All tests succeeded" << endl;
}
