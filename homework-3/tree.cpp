int countContains(const double a1[], int n1, const double a2[], int n2) {
    if (n2 <= 0) // C(n1, 0)
        return 1;
    if (n1 < n2)
        return 0;
    int total = countContains(a1 + 1, n1 - 1, a2, n2);
    if (*a1 == *a2)
        total += countContains(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
    return total;
}

void exchange(double &x, double &y) {
    double t = x;
    x = y;
    y = t;
}

void separate(double a[], int n, double separator, int &firstNotGreater, int &firstLess) {
    if (n < 0)
        n = 0;

    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess) {
        if (a[firstUnknown] < separator) {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        } else {
            if (a[firstUnknown] > separator) {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}

void order(double a[], int n) {
    if (n <= 1)
        return;
    int firstNotGreater, firstLess;
    separate(a, n, *a, firstNotGreater, firstLess);
    order(a, firstNotGreater);
    order(a + firstLess, n - firstLess);
}
