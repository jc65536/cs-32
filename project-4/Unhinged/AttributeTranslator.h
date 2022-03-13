#ifndef ATTRIBUTETRANSLATOR_H
#define ATTRIBUTETRANSLATOR_H

#include <string>
#include <list>
#include <vector>

#include "provided.h"
#include "RadixTree.h"

class AttributeTranslator {
public:
    bool Load(std::string filename);
    std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair &source) const;

    // REMOVE BEFORE SUBMISSION
    void print();

private:
    std::list<AttValPair> attValPairs;

    // The first pointer in the list is a pointer to itself in attValPairs
    RadixTree<std::list<AttValPair *>> attValTree;
};

#endif // Include guard
