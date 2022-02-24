#include <cassert>
#include <iostream>
#include <list>
#include <vector>
using namespace std;

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE END OF THE smallbig FUNCTION

bool smallbig(vector<int> &c1, list<int> &c2) {
    if (c1.size() != c2.size())
        return false;

    int i = 0, size = c1.size();
    auto it = c2.begin(), end = c2.end();
    for (; i < size && it != end; i++, it++) {
        int n1 = c1[i], n2 = *it;
        if (n1 > n2) {
            c1[i] = n2;
            *it = n1;
        }
    }

    return true;
}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE START OF THE smallbig FUNCTION

int main() {
    {
        vector<int> v;
        list<int> li;
        assert(smallbig(v, li));

        int x[4] = {7, -5, 3, 6};
        int y[4] = {4, 1, 3, 2};
        int expectx[4] = {4, -5, 3, 2};
        int expecty[4] = {7, 1, 3, 6};
        vector<int> small(x, x + 4);
        list<int> big(y, y + 4);
        assert(smallbig(small, big));
        vector<int> expectsmall(expectx, expectx + 4);
        list<int> expectbig(expecty, expecty + 4);
        assert(small == expectsmall && big == expectbig);

        list<int> big2(y, y + 3);
        assert(!smallbig(small, big2)); // different num of elements
    }

    {
        vector<int> v = {0, 0, 0};
        list<int> li = {0, 0, 0};
        vector<int> ev = {0, 0, 0};
        list<int> eli = {0, 0, 0};
        assert(smallbig(v, li));
        assert(v == ev && li == eli);
    }

    {
        vector<int> v = {1, 2, 3};
        list<int> li = {2, 3, 4};
        vector<int> ev = {1, 2, 3};
        list<int> eli = {2, 3, 4};
        assert(smallbig(v, li));
        assert(v == ev && li == eli);
    }

    cout << "All tests succeeded" << endl;
}
