#ifndef MAP
#define MAP

#include <string>

#define INT_TO_STRING 0

#if INT_TO_STRING
#else
using KeyType = std::string;
using ValueType = double;
#endif

class Map {
public:
    Map(); // Create an empty map (i.e., one with no key/value pairs)

    Map(const Map &m);

    ~Map();

    Map &operator=(Map m);

    bool empty() const; // Return true if the map is empty, otherwise false.

    int size() const; // Return the number of key/value pairs in the map.

    bool insert(const KeyType &key, const ValueType &value);
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map, or the map has a fixed
    // capacity and is full).

    bool update(const KeyType &key, const ValueType &value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value that it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.

    bool insertOrUpdate(const KeyType &key, const ValueType &value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value that it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that the key is not already in the map and the map has a fixed
    // capacity and is full).

    bool erase(const KeyType &key);
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.

    bool contains(const KeyType &key) const;
    // Return true if key is equal to a key currently in the map, otherwise
    // false.

    bool get(const KeyType &key, ValueType &value) const;
    // If key is equal to a key currently in the map, set value to the
    // value in the map which that key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.

    bool get(int i, KeyType &key, ValueType &value) const;
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of the key/value pair in the map whose key is strictly
    // greater than exactly i keys in the map and return true.  Otherwise,
    // leave the key and value parameters unchanged and return false.

    void swap(Map &other);
    // Exchange the contents of this map with the other one.

    void dump() const;

private:
    struct Node {
        KeyType key;
        ValueType value;
        Node *prev, *next;

        Node() {}
        Node(KeyType k, ValueType v) : key(k), value(v) {}
    };

    int count;
    Node * dm; // dm->next is the head of the list; dm->prev is the tail of the list

    // Sets n to point to the first Node whose key >= key (or dm if no such Node exists)
    // Returns whether an exact match was found
    bool findNode(const KeyType &key, Node *&n) const;
    
    // to will come after n after the insertion
    void insert(Node *n, Node *to);

    void erase(Node *n);
};

bool merge(const Map& m1, const Map& m2, Map& result);

void reassign(const Map& m, Map& result);

#endif // Header guard
