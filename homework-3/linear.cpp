bool anyFalse(const double a[], int n) {
    return n > 0 && (!somePredicate(*a) || anyFalse(++a, --n));
}

int countFalse(const double a[], int n) {
    return n > 0 ? !somePredicate(*a) + countFalse(a + 1, --n) : 0;
}

int firstTrue(const double a[], int n) {
    return n > 0 ? n = firstTrue(a + 1, --n), (n += n >= 0) * !somePredicate(*a) : -1;
}

int locateMax(const double a[], int n) {
    return n > 0 ? n = locateMax(a + 1, --n), *a < a[++n] ? n : 0 : -1;
}

bool contains(const double a1[], int n1, const double a2[], int n2) {
    return n2 <= 0 || n1 >= n2 && (*a1 - *a2 || ++a2 - --n2, contains(++a1, --n1, a2, n2));
}
