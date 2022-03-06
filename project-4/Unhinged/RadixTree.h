#ifndef RADIXTREE_H
#define RADIXTREE_H

#include <string>

template <class ValueType>
class RadixTree {
public:
    RadixTree();
    ~RadixTree();
    void insert(std::string key, const ValueType &value);
    ValueType *search(std::string key) const;
};

#endif // Include guard
