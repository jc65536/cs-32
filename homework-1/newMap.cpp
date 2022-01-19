#include <iostream>

#include "newMap.h"

Map::Map() : Map(DEFAULT_MAX_ITEMS) {}

Map::Map(int capacity) : capacity(capacity), count(0) {
    list = new Pairing[capacity];
}

Map::Map(const Map &m) : Map(m.capacity) {
    count = m.count;
    for (int i = 0; i < count; i++) {
        list[i] = m.list[i];
    }
}

Map::~Map() {
    delete[] list;
}

Map &Map::operator=(Map m) {
    swap(m);
    return *this;
}

void Map::swap(Map &other) {
    Pairing * const l = list;
    list = other.list;
    other.list = l;
    const int cap = capacity;
    capacity = other.capacity;
    other.capacity = cap;
    const int cnt = count;
    count = other.count;
    other.count = cnt;
}

bool Map::empty() const {
    return count == 0;
}

int Map::size() const {
    return count;
}

bool Map::contains(const KeyType &key) const {
    return findIndex(key) >= 0;
}

bool Map::get(const KeyType &key, ValueType &value) const {
    const int i = findIndex(key);
    if (i < 0) return false;
    value = list[i].value;
    return true;
}

bool Map::get(int i, KeyType &key, ValueType &value) const {
    if (i < 0 || i >= count) return false;
    const Pairing &p = list[i];
    key = p.key;
    value = p.value;
    return true;
}

void Map::dump() const {
    std::cerr << "(" << count << ") {";
    for (int i = 0; i < count; i++) {
        const Pairing &p = list[i];
        std::cerr << p.key << ": " << p.value;
        if (i < count - 1)
            std::cerr << ", ";
    }
    std::cerr << "}" << std::endl;
}

bool Map::insert(const KeyType &key, const ValueType &value) {
    if (count >= capacity) return false;
    int i = findIndex(key);
    if (i >= 0) return false;
    i = -(i + 1);
    for (int j = count; j > i; j--) {
        list[j] = list[j - 1];
    }
    list[i] = Pairing{key, value};
    count++;
    return true;
}

bool Map::update(const KeyType &key, const ValueType &value) {
    const int i = findIndex(key);
    if (i < 0) return false;
    list[i].value = value;
    return true;
}

bool Map::insertOrUpdate(const KeyType &key, const ValueType &value) {
    return insert(key, value) || update(key, value);
}

bool Map::erase(const KeyType &key) {
    const int i = findIndex(key);
    if (i < 0) return false;
    for (int j = i; j + 1 < count; j++) {
        list[j] = list[j + 1];
    }
    count--;
    return true;
}

// Private functions

int Map::findIndex(const KeyType &key) const {
    int min = -1, mid, max = count;
    while (max - min > 1) {
        mid = (min + max) / 2;
        const KeyType &midKey = list[mid].key;
        if (midKey == key) {
            return mid;
        } else if (midKey < key) {
            min = mid;
        } else {
            max = mid;
        }
    }
    return -1 - max;
}
