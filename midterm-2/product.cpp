#include <iostream>
#include <cassert>
using namespace std;

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE END OF THE product FUNCTION

int product(int a, int b) {
    if (a == 0 || b == 0)
        return 0;
    if (a < 0)
        return -product(-a, b);
    if (b < 0)
        return -product(a, -b);
    return a + product(a, --b);
}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AT THE START OF THE product FUNCTION

int main()
{
    assert(product(7, 5) == 35);
    assert(product(-4, 8) == -32);
    assert(product(0, 0) == 0);
    assert(product(0, 4) == 0);
    assert(product(-5, 0) == 0);
    assert(product(-5, -5) == 25);
    assert(product(500, 7821) == 500 * 7821);
    assert(product(-21313, 32) == -21313 * 32);

    cout << "All tests succeeded" << endl;
}
