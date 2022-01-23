#include <iostream>

#include "Map.h"

// Non-member functions

bool merge(const Map& m1, const Map& m2, Map& result) {
    // Some shortcuts in case of aliases
    if (&result == &m1 && &result == &m2)
        return true;
    
    if (&m1 == &m2) {
        result = Map(m1);
        return true;
    }

    // Guarantees that result is always independent from m1 or m2
    if (&result == &m1)
        return merge(Map(m1), m2, result);
    if (&result == &m2)
        return merge(m1, Map(m2), result);

    result = Map(m1);
    int s1 = m1.size(), s2 = m2.size();
    KeyType key;
    ValueType v1, v2;
    bool ret = true;

    for (int i = 0; i < s1; i++) {
        m1.get(i, key, v1);
        if (m2.get(key, v2) && v1 != v2) {
            result.erase(key);
            ret = false;
        }
    }

    for (int j = 0; j < s2; j++) {
        m2.get(j, key, v2);
        if (!m1.get(key, v1)) {
            result.insert(key, v2);
        }
    }

    return ret;
}

void reassign(const Map& m, Map& result) {
    if (&result != &m)
        result = Map(m);

    int s = result.size();
    if (s <= 1)
        return;

    KeyType k1, k2;
    ValueType firstVal, val;
    result.get(0, k1, firstVal);
    for (int i = 1; i < s; i++) {
        result.get(i, k2, val);
        result.update(k1, val);
        k1 = k2;
    }
    result.update(k2, firstVal);
}

// Public member functions

Map::Map() : count(0), dm(new Node) {
    dm->next = dm->prev = dm;
}

Map::Map(const Map &m) : Map() {
    Node *n = m.dm->next, *newNode;
    while (n != m.dm) {
        // I was going to use a copy constructor, but this works too
        newNode = new Node(n->key, n->value);
        insert(newNode, dm);
        n = n->next;
    }
}

Map::~Map() {
    dm->prev->next = nullptr;
    while (dm->next) {
        dm = dm->next;
        delete dm->prev;
    }
    delete dm;
}

Map &Map::operator=(Map m) {
    swap(m);
    return *this;
}

bool Map::empty() const {
    return count == 0;
}

int Map::size() const {
    return count;
}

bool Map::contains(const KeyType &key) const {
    Node *n;
    return findNode(key, n);
}

bool Map::get(const KeyType &key, ValueType &value) const {
    Node *n;
    if (!findNode(key, n)) return false;
    value = n->value;
    return true;
}

bool Map::get(int i, KeyType &key, ValueType &value) const {
    if (i < 0 || i >= count) return false;
    Node *n = dm->next;
    for (int j = 0; j < i; j++)
        n = n->next;
    key = n->key;
    value = n->value;
    return true;
}

void Map::dump() const {
    std::cerr << "(" << count << ") {";
    for (const Node *n = dm->next; n != dm; n = n->next) {
        std::cerr << n->key << ": " << n->value;
        if (n->next != dm)
            std::cerr << ", ";
    }
    std::cerr << "}" << std::endl;
}

bool Map::insert(const KeyType &key, const ValueType &value) {
    Node *n;
    if (findNode(key, n)) return false;
    Node *newNode = new Node(key, value);
    insert(newNode, n);
    return true;
}

bool Map::update(const KeyType &key, const ValueType &value) {
    Node *n;
    if (!findNode(key, n)) return false;
    n->value = value;
    return true;
}

bool Map::insertOrUpdate(const KeyType &key, const ValueType &value) {
    return insert(key, value) || update(key, value);
}

bool Map::erase(const KeyType &key) {
    Node *n;
    if (!findNode(key, n)) return false;
    erase(n);
    return true;
}

void Map::swap(Map &other) {
    Node *const tdm = dm;
    dm = other.dm;
    other.dm = tdm;
    const int tc = count;
    count = other.count;
    other.count = tc;
}

// Private functions

bool Map::findNode(const KeyType &key, Node *&n) const {
    n = dm->next;
    while (n != dm && n->key < key)
        n = n->next;
    return n != dm && n->key == key;
}

void Map::insert(Node *n, Node *to) {
    n->prev = to->prev;
    n->next = to;
    n->prev->next = n;
    to->prev = n;
    count++;
}

void Map::erase(Node *n) {
    n->prev->next = n->next;
    n->next->prev = n->prev;
    delete n;
    count--;
}
