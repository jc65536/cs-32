#include <cassert>
#include <iostream>
#include <string>
using namespace std;

bool somePredicate(double x) {
    return x;
}

#include "sport.cpp"
#include "linear.cpp"
#include "tree.cpp"

void display(const Sport *sp) {
    cout << sp->name() << " is ";
    if (sp->isOutdoor())
        cout << "an outdoor sport ";
    else
        cout << "an indoor sport ";
    cout << "with an icon showing " << sp->icon() << "." << endl;
}

int main() {
    Sport *sports[4];
    sports[0] = new Snowboarding("Women's halfpipe");
    // Biathlon events have a distance in kilometers
    sports[1] = new Biathlon("Men's pursuit", 12.5);
    sports[2] = new Biathlon("Women's sprint", 7.5);
    sports[3] = new FigureSkating("Pair skating free");

    cout << "Here are the sports." << endl;
    for (int k = 0; k < 4; k++)
        display(sports[k]);

    // Clean up the sports before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
        delete sports[k];
    
    //==========================================================================
    // bool anyFalse(const double a[], int n);

    // No false
    {
        double a[] = {1, 1, 1, 1, 1};
        int n = 5;
        assert(!anyFalse(a, n));
    }

    // Some false
    {
        double a[] = {1, 0, 1, 1, 1};
        int n = 5;
        assert(anyFalse(a, n));
    }

    // false at beginning
    {
        double a[] = {0, 1, 1, 1, 1};
        int n = 5;
        assert(anyFalse(a, n));
    }

    // false at end
    {
        double a[] = {1, 1, 1, 1, 0};
        int n = 5;
        assert(anyFalse(a, n));
    }

    // n = 0
    {
        double a[0];
        int n = 0;
        assert(!anyFalse(a, n));
    }

    // n < 0
    {
        double a[0];
        int n = -1;
        assert(!anyFalse(a, n));
    }

    //==========================================================================
    // int countFalse(const double a[], int n);

    // No false
    {
        double a[] = {1, 1, 1, 1, 1};
        int n = 5;
        assert(countFalse(a, n) == 0);
    }

    // One false
    {
        double a[] = {1, 1, 0, 1, 1};
        int n = 5;
        assert(countFalse(a, n) == 1);
    }

    // Some false
    {
        double a[] = {1, 1, 0, 0, 1};
        int n = 5;
        assert(countFalse(a, n) == 2);
    }

    // All false
    {
        double a[] = {0, 0, 0, 0, 0};
        int n = 5;
        assert(countFalse(a, n) == 5);
    }

    // n = 0
    {
        double a[0];
        int n = 0;
        assert(countFalse(a, n) == 0);
    }

    // n < 0
    {
        double a[0];
        int n = -1;
        assert(countFalse(a, n) == 0);
    }

    //==========================================================================
    // int firstTrue(const double a[], int n);

    // No true
    {
        double a[] = {0, 0, 0, 0, 0};
        int n = 5;
        assert(firstTrue(a, n) == -1);
    }

    // One true
    {
        double a[] = {0, 0, 0, 1, 0};
        int n = 5;
        assert(firstTrue(a, n) == 3);
    }

    // Multiple trues
    {
        double a[] = {0, 1, 1, 0, 0};
        int n = 5;
        assert(firstTrue(a, n) == 1);
    }

    // true at beginning
    {
        double a[] = {1, 0, 0, 0, 0};
        int n = 5;
        assert(firstTrue(a, n) == 0);
    }

    // true at end
    {
        double a[] = {0, 0, 0, 0, 1};
        int n = 5;
        assert(firstTrue(a, n) == 4);
    }

    // n = 0
    {
        double a[0];
        int n = 0;
        assert(firstTrue(a, n) == -1);
    }

    // n < 0
    {
        double a[0];
        int n = -1;
        assert(firstTrue(a, n) == -1);
    }

    //==========================================================================
    // int locateMax(const double a[], int n);

    // Single largest
    {
        double a[] = {1, 2, 3, 4, 2};
        int n = 5;
        assert(locateMax(a, n) == 3);
    }

    // Multiple largest
    {
        double a[] = {1, 4, 2, 4, 3};
        int n = 5;
        assert(locateMax(a, n) == 1);
    }

    // Largest in beginning
    {
        double a[] = {5, 4, 3, 2, 1};
        int n = 5;
        assert(locateMax(a, n) == 0);
    }

    // Largest at end
    {
        double a[] = {1, 2, 3, 4, 5};
        int n = 5;
        assert(locateMax(a, n) == 4);
    }

    // All elements equal
    {
        double a[] = {1, 1, 1, 1, 1};
        int n = 5;
        assert(locateMax(a, n) == 0);
    }

    // n = 1
    {
        double a[] = {1};
        int n = 1;
        assert(locateMax(a, n) == 0);
    }

    // n = 0
    {
        double a[0];
        int n = 0;
        assert(locateMax(a, n) == -1);
    }

    // n < 0
    {
        double a[0];
        int n = -1;
        assert(locateMax(a, n) == -1);
    }

    //==========================================================================
    // bool contains(const double a1[], int n1, const double a2[], int n2);

    // a1 contains a2
    {
        double a1[] = {1, 2, 3, 4, 5};
        int n1 = 5;
        double a2[] = {2, 4, 5};
        double n2 = 3;
        assert(contains(a1, n1, a2, n2));
    }

    // a1 = a2
    {
        double a[] = {1, 2, 3, 4, 5};
        int n = 5;
        assert(contains(a, n, a, n));
    }

    // a2 contains an element not in a1
    {
        double a1[] = {1, 2, 3, 4, 5};
        int n1 = 5;
        double a2[] = {2, 4, 6};
        double n2 = 3;
        assert(!contains(a1, n1, a2, n2));
    }

    // n2 > n1
    {
        double a1[] = {1, 2, 3};
        int n1 = 3;
        double a2[] = {1, 2, 2, 3, 3};
        int n2 = 5;
        assert(!contains(a1, n1, a2, n2));
    }

    // n2 = 0
    {
        double a1[] = {1, 2, 3};
        int n1 = 3;
        double a2[0];
        int n2 = 0;
        assert(contains(a1, n1, a2, n2));
    }

    // n1 = 0
    {
        double a1[0];
        int n1 = 0;
        double a2[] = {1, 2, 3};
        int n2 = 3;
        assert(!contains(a1, n1, a2, n2));
    }

    // n1 = n2 = 0
    {
        double a1[0];
        int n1 = 0;
        double a2[0];
        int n2 = 0;
        assert(contains(a1, n1, a2, n2));
    }

    // n2 < 0
    {
        double a1[] = {1, 2, 3};
        int n1 = 3;
        double a2[0];
        int n2 = -1;
        assert(contains(a1, n1, a2, n2));
    }

    // n1 < 0
    {
        double a1[0];
        int n1 = -1;
        double a2[] = {1, 2, 3};
        int n2 = 3;
        assert(!contains(a1, n1, a2, n2));
    }

    // n1 < 0 and n2 < 0
    {
        double a1[0];
        int n1 = -1;
        double a2[0];
        int n2 = -1;
        assert(contains(a1, n1, a2, n2));
    }

    //==========================================================================
    // int countContains(const double a1[], int n1, const double a2[], int n2);

    {
        double a1[] = {10, 50, 40, 20, 50, 40, 30};
        int n1 = 7;
        double a2[] = {10, 20, 40};
        int n2 = 3;
        assert(countContains(a1, n1, a2, n2) == 1);        
    }

    {
        double a1[] = {10, 50, 40, 20, 50, 40, 30};
        int n1 = 7;
        double a2[] = {10, 40, 30};
        int n2 = 3;
        assert(countContains(a1, n1, a2, n2) == 2);        
    }

    {
        double a1[] = {10, 50, 40, 20, 50, 40, 30};
        int n1 = 7;
        double a2[] = {20, 10, 40};
        int n2 = 3;
        assert(countContains(a1, n1, a2, n2) == 0);        
    }

    {
        double a1[] = {10, 50, 40, 20, 50, 40, 30};
        int n1 = 7;
        double a2[] = {20, 10, 40};
        int n2 = 3;
        assert(countContains(a1, n1, a2, n2) == 0);        
    }

    {
        double a1[] = {10, 50, 40, 20, 50, 40, 30};
        int n1 = 7;
        double a2[] = {50, 40, 30};
        int n2 = 3;
        assert(countContains(a1, n1, a2, n2) == 3);        
    }

    // Worst case
    {
        double a1[] = {0, 0, 0, 0, 0, 0, 0};
        int n1 = 7;
        double a2[] = {0, 0, 0};
        int n2 = 3;
        assert(countContains(a1, n1, a2, n2) == 35);        
    }

    // n1 = n2 = 0
    {
        double a1[0];
        int n1 = 0;
        double a2[0];
        int n2 = 0;
        assert(countContains(a1, n1, a2, n2) == 1);        
    }

    // n1 = n2 < 0
    {
        double a1[0];
        int n1 = -1;
        double a2[0];
        int n2 = -1;
        assert(countContains(a1, n1, a2, n2) == 1);        
    }

    //==========================================================================
    // void order(double a[], int n);

    {
        double a[] = {3, 4, 1, 5, 2, 4};
        double n = 6;
        order(a, n);
        for (int i = 0; i + 1 < n; i++) {
            assert(a[i] >= a[i + 1]);
        }
    }

    // n = 0
    {
        double a[0];
        double n = 0;
        order(a, n);
    }

    // n < 0
    {
        double a[0];
        double n = -1;
        order(a, n);
    }


    cout << "Passed all tests." << endl;
}
