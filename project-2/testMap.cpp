#include <cassert>
#include <iostream>

#include "Map.h"

#if INT_TO_STRING
KeyType k(int i) {
    return i;
}

ValueType v(int i) {
    ValueType s = "aa";
    s[0] += i / 26;
    s[1] += i % 26;
    return s;
}
#else
KeyType k(int i) {
    KeyType s = "aa";
    s[0] += i / 26;
    s[1] += i % 26;
    return s;
}

ValueType v(int i) {
    return i;
}
#endif

int main() {
    //==========================================================================
    // bool empty() const;
    {
        Map m;

        // Empty
        assert(m.empty());

        // Non-empty
        m.insert(k(0), v(0));
        assert(!m.empty());
    }

    //==========================================================================
    // int size() const;
    {
        Map m;

        // 0 size
        assert(m.size() == 0);

        // Some size
        m.insert(k(2), v(2));
        for (int i = 0; i < 5; i++)
            m.insert(k(i), v(i));
        assert(m.size() == 5);

        // Size doesn't become negative
        m.erase(k(2));
        for (int i = 0; i < 99; i++)
            m.erase(k(i));
        assert(m.size() == 0);
    }

    //==========================================================================
    // bool contains(const KeyType &key) const;
    {
        Map m;
        for (int i = 0; i < 5; i++)
            m.insert(k(i), v(i));

        // Key exists
        assert(m.contains(k(2)));

        // Key doesn't exist
        m.erase(k(2));
        assert(!m.contains(k(2)));
    }

    //==========================================================================
    // bool get(const KeyType &key, ValueType &value) const;
    {
        Map m;
        ValueType value = v(-1);

        // Key doesn't exist
        assert(!m.get(k(0), value));
        assert(value == v(-1));

        // Key exists
        m.insert(k(0), v(0));
        assert(m.get(k(0), value));
        assert(value == v(0));
    }

    //==========================================================================
    // bool get(int i, KeyType &key, ValueType &value) const;
    {
        Map m;
        KeyType key = k(-1);
        ValueType value = v(-1);

        // Empty map
        assert(!m.get(0, key, value));
        assert(key == k(-1) && value == v(-1));

        // i < 0
        for (int i = 0; i < 5; i++)
            m.insert(k(i), v(i));
        assert(!m.get(-1, key, value));
        assert(key == k(-1) && value == v(-1));

        // i >= size
        assert(!m.get(5, key, value));
        assert(key == k(-1) && value == v(-1));

        // Able to get
        assert(m.get(2, key, value));
        assert(key == k(2) && value == v(2));
    }

    //==========================================================================
    // bool insert(const KeyType &key, const ValueType &value);
    {
        Map m;

        // Able to add
        assert(m.insert(k(0), v(0)));
        m.dump(); // (1) {aa: 0}

        // Added pairings are sorted by key
        for (int i = 4; i > 0; i -= 2)
            m.insert(k(i), v(i));
        for (int i = 1; i < 5; i += 2)
            m.insert(k(i), v(i));
        m.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}

        // Can't add due to conflict
        assert(!m.insert(k(0), v(1)));
        m.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}
    }

    //==========================================================================
    // bool update(const KeyType &key, const ValueType &value);
    {
        Map m;
        for (int i = 0; i < 5; i++)
            m.insert(k(i), v(i));

        // Able to update
        assert(m.update(k(2), v(99)));
        m.dump(); // (5) {aa: 0, ab: 1, ac: 99, ad: 3, ae: 4}

        // Can't update
        assert(!m.update(k(99), v(99)));
        m.dump(); // (5) {aa: 0, ab: 1, ac: 99, ad: 3, ae: 4}
    }

    //==========================================================================
    // bool insertOrUpdate(const KeyType &key, const ValueType &value);
    {
        Map m;
        for (int i = 1; i < 5; i++)
            m.insert(k(i), v(i));

        // Insert
        assert(m.insertOrUpdate(k(0), v(0)));
        m.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}

        // Update
        assert(m.insertOrUpdate(k(2), v(99)));
        m.dump(); // (5) {aa: 0, ab: 1, ac: 99, ad: 3, ae: 4}
    }

    //==========================================================================
    // bool erase(const KeyType &key);
    {
        Map m;
        m.insert(k(0), v(0));

        // Key exists
        assert(m.erase(k(0)));
        m.dump(); // (0) {}

        // Key doesn't exist
        assert(!m.erase(k(2)));
        m.dump(); // (0) {}
    }

    //==========================================================================
    // void swap(Map &other);
    {
        Map m, n;
        for (int i = 0; i < 3; i++)
            m.insert(k(i), v(i));
        for (int i = 3; i < 5; i++)
            n.insert(k(i), v(i));

        // Swap works
        m.dump(); // (3) {aa: 0, ab: 1, ac: 2}
        n.dump(); // (2) {ad: 3, ae: 4}
        m.swap(n);
        m.dump(); // (2) {ad: 3, ae: 4}
        n.dump(); // (3) {aa: 0, ab: 1, ac: 2}

        // Ensure swapped Maps are independent
        for (int i = 0; i < 3; i++)
            n.update(k(i), v(99));
        m.dump(); // (2) {ad: 3, ae: 4}
        n.dump(); // (3) {aa: 99, ab: 99, ac: 99}
    }

    //==========================================================================
    // Copy constructor
    {
        Map m;
        for (int i = 0; i < 5; i++)
            m.insert(k(i), v(i));

        // Copy works
        Map n(m);
        m.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}
        n.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}

        // Changes to the copy don't affect the original
        n.insertOrUpdate(k(2), v(99));
        n.erase(k(0));
        m.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}
        n.dump(); // (5) {ab: 1, ac: 99, ad: 3, ae: 4}
    }

    //==========================================================================
    // Copy assignment operator
    {
        Map m, n;
        for (int i = 0; i < 3; i++)
            m.insert(k(i), v(i));
        for (int i = 3; i < 5; i++)
            n.insert(k(i), v(i));

        // Copy works
        m.dump(); // (3) {aa: 0, ab: 1, ac: 2}
        n.dump(); // (2) {ad: 3, ae: 4}
        m = n;
        m.dump(); // (2) {ad: 3, ae: 4}
        n.dump(); // (2) {ad: 3, ae: 4}

        // Changes to the copy don't affect original
        n.insert(k(2), v(2));
        for (int i = 2; i < 5; i++)
            n.erase(k(i));
        m.dump(); // (2) {ad: 3, ae: 4}
        n.dump(); // (0) {}
    }

    //==========================================================================
    // bool merge(const Map& m1, const Map& m2, Map& result);
    {
        Map m, n, res;

        // m1, m2 are disjoint
        for (int i = 0; i < 3; i++)
            m.insert(k(i), v(i));
        for (int i = 3; i < 5; i++)
            n.insert(k(i), v(i));
        assert(merge(m, n, res));
        res.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}

        // m1, m2 overlap, and overlapped pairings are equal
        n.insert(k(2), v(2));
        assert(merge(m, n, res));
        res.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}

        // m1, m2 overlap, and some overlapped parings aren't equal
        n.insert(k(1), v(99));
        assert(!merge(m, n, res));
        res.dump(); // (4) {aa: 0, ac: 2, ad: 3, ae: 4}

        // m1, m2 completely overlap, with no pairings equal
        n = Map();
        for (int i = 0; i < 3; i++)
            n.insert(k(i), v(99));
        assert(!merge(m, n, res));
        res.dump(); // (0) {}

        // m1 is empty
        n = Map();
        assert(merge(n, m, res));
        res.dump(); // (3) {aa: 0, ab: 1, ac: 2}

        // m2 is empty
        assert(merge(m, n, res));
        res.dump(); // (3) {aa: 0, ab: 1, ac: 2}

        // m1 and m2 are empty
        assert(merge(n, n, res));
        res.dump(); // (0) {}

        // m1 is m2 is result
        assert(merge(m, m, m));
        m.dump(); // (3) {aa: 0, ab: 1, ac: 2}

        // m1 is result
        n = Map();
        for (int i = 2; i < 5; i++)
            n.insert(k(i), v(i));
        n.insert(k(1), v(99));
        assert(!merge(m, n, m));
        m.dump(); // (4) {aa: 0, ac: 2, ad: 3, ae: 4}

        // m2 is result
        m = Map();
        for (int i = 0; i < 3; i++)
            m.insert(k(i), v(i));
        assert(!merge(m, n, n));
        n.dump(); // (4) {aa: 0, ac: 2, ad: 3, ae: 4}

        // m1 is m2
        res = Map();
        assert(merge(m, m, res));
        res.dump(); // (3) {aa: 0, ab: 1, ac: 2}
    }

    //==========================================================================
    // void reassign(const Map& m, Map& result);
    {
        Map m, res;

        // Typical case
        for (int i = 0; i < 3; i++)
            m.insert(k(i), v(i));
        m.update(k(0), v(1));
        reassign(m, res);
        res.dump(); // (3) {aa: 1, ab: 2, ac: 1}

        // result is non-empty
        m = Map();
        for (int i = 0; i < 4; i++)
            m.insert(k(i), v(i));
        reassign(m, res);
        res.dump(); // (4) {aa: 1, ab: 2, ac: 3, ad: 0}

        // m has only one element
        m = Map();
        m.insert(k(0), v(0));
        reassign(m, res);
        res.dump(); // (1) {aa: 0}

        // m is empty
        m = Map();
        reassign(m, res);
        res.dump(); // (0) {}

        // m is result
        for (int i = 0; i < 3; i++)
            m.insert(k(i), v(i));
        for (int i = 0; i < 3; i++) {
            reassign(m, m);
            m.dump();
        }
        /*
        (3) {aa: 1, ab: 2, ac: 0}
        (3) {aa: 2, ab: 0, ac: 1}
        (3) {aa: 0, ab: 1, ac: 2}
        */
    }
}
