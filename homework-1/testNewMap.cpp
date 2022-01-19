#include <cassert>

#include "newMap.h"

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
    /*
    (1) {aa: 0}
    (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}
    (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}
    (5) {aa: 0, ab: 1, ac: 99, ad: 3, ae: 4}
    (5) {aa: 0, ab: 1, ac: 99, ad: 3, ae: 4}
    (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}
    (5) {aa: 0, ab: 1, ac: 99, ad: 3, ae: 4}
    (0) {}
    (0) {}
    (3) {aa: 0, ab: 1, ac: 2}
    (2) {ad: 3, ae: 4}
    (2) {ad: 3, ae: 4}
    (3) {aa: 0, ab: 1, ac: 2}
    */

    Map m, n;
    KeyType key;
    ValueType value;

    // bool empty() const;
    m = Map();

    // Empty
    assert(m.empty());

    // Non-empty
    m.insert(k(0), v(0));
    assert(!m.empty());

    // int size() const;
    m = Map();

    // 0 size
    assert(m.size() == 0);

    // Some size
    m.insert(k(2), v(2));
    for (int i = 0; i < 14; i++)
        m.insert(k(i), v(i));
    assert(m.size() == 14);

    // Size doesn't grow beyond DEFAULT_MAX_ITEMS
    for (int i = 0; i < 2 * DEFAULT_MAX_ITEMS; i++)
        m.insert(k(i), v(i));
    assert(m.size() == DEFAULT_MAX_ITEMS);

    // Size doesn't become negative
    m.erase(k(2));
    for (int i = 0; i < 2 * DEFAULT_MAX_ITEMS; i++)
        m.erase(k(i));
    assert(m.size() == 0);

    // bool contains(const KeyType &key) const;
    m = Map();
    for (int i = 0; i < 5; i++)
        m.insert(k(i), v(i));

    // Key exists
    assert(m.contains(k(2)));

    // Key doesn't exist
    m.erase(k(2));
    assert(!m.contains(k(2)));

    // bool get(const KeyType &key, ValueType &value) const;
    m = Map();
    value = v(-1);

    // Key doesn't exist
    assert(!m.get(k(0), value));
    assert(value == v(-1));

    // Key exists
    m.insert(k(0), v(0));
    assert(m.get(k(0), value));
    assert(value == v(0));

    // bool get(int i, KeyType &key, ValueType &value) const;
    m = Map();
    key = k(-1);
    value = v(-1);

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

    // bool insert(const KeyType &key, const ValueType &value);
    m = Map();

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

    // Can't add due to size
    for (int i = 5; i < DEFAULT_MAX_ITEMS; i++)
        m.insert(k(i), v(i));
    assert(!m.insert(k(DEFAULT_MAX_ITEMS + 1), v(DEFAULT_MAX_ITEMS + 1)));

    // bool update(const KeyType &key, const ValueType &value);
    m = Map();
    for (int i = 0; i < 5; i++)
        m.insert(k(i), v(i));

    // Able to update
    assert(m.update(k(2), v(99)));
    m.dump(); // (5) {aa: 0, ab: 1, ac: 99, ad: 3, ae: 4}

    // Can't update
    assert(!m.update(k(99), v(99)));
    m.dump(); // (5) {aa: 0, ab: 1, ac: 99, ad: 3, ae: 4}

    // bool insertOrUpdate(const KeyType &key, const ValueType &value);
    m = Map();
    for (int i = 1; i < 5; i++)
        m.insert(k(i), v(i));

    // Insert
    assert(m.insertOrUpdate(k(0), v(0)));
    m.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}

    // Update
    assert(m.insertOrUpdate(k(2), v(99)));
    m.dump(); // (5) {aa: 0, ab: 1, ac: 99, ad: 3, ae: 4}

    // Can't insert nor update
    for (int i = 5; i < DEFAULT_MAX_ITEMS; i++)
        m.insert(k(i), v(i));
    assert(!m.insertOrUpdate(k(DEFAULT_MAX_ITEMS + 1), v(DEFAULT_MAX_ITEMS + 1)));

    // bool erase(const KeyType &key);
    m = Map();
    m.insert(k(0), v(0));

    // Key exists
    assert(m.erase(k(0)));
    m.dump(); // (0) {}

    // Key doesn't exist
    assert(!m.erase(k(2)));
    m.dump(); // (0) {}

    // void swap(Map &other);
    m = n = Map();
    for (int i = 0; i < 3; i++)
        m.insert(k(i), v(i));
    for (int i = 3; i < 5; i++)
        n.insert(k(i), v(i));
    m.dump(); // (3) {aa: 0, ab: 1, ac: 2}
    n.dump(); // (2) {ad: 3, ae: 4}
    m.swap(n);
    m.dump(); // (2) {ad: 3, ae: 4}
    n.dump(); // (3) {aa: 0, ab: 1, ac: 2}

    // New functions
    Map a;
    a.dump(); // (0) {}
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
        a.insert(k(i), v(i));
    assert(a.size() == DEFAULT_MAX_ITEMS);
    assert(!a.insert(k(DEFAULT_MAX_ITEMS + 1), v(DEFAULT_MAX_ITEMS + 1)));

    Map b(5);
    b.dump(); // (0) {}
    for (int i = 0; i < 5; i++)
        b.insert(k(i), v(i));
    assert(b.size() == 5);
    assert(!b.insert(k(6), v(6)));
    b.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}

    Map c(b);
    c.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}
    c.update(k(2), v(99));
    c.dump(); // (5) {aa: 0, ab: 1, ac: 99, ad: 3, ae: 4}
    b.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}

    a = b;
    a.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}
    a.update(k(2), v(99));
    a.dump(); // (5) {aa: 0, ab: 1, ac: 99, ad: 3, ae: 4}
    b.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}

    a = Map(2);
    a.swap(b);
    a.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}
    b.dump(); // (0) {}
    for (int i = 0; i < 5; i++) {
        b.insert(k(i), v(i));
        a.erase(k(i));
    }
    assert(a.empty());
    assert(b.size() == 2);
    b.dump(); // (2) {aa: 0, ab: 1}
    for (int i = 0; i < 10; i++) {
        a.insert(k(i), v(i));
    }
    assert(a.size() == 5);
    a.dump(); // (5) {aa: 0, ab: 1, ac: 2, ad: 3, ae: 4}
}
