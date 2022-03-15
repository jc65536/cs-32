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
    static std::string listToString(const std::list<AttValPair *> &l) {
        std::string s = "{";
        for (AttValPair *p : l)
            s += attValToString(*p) + ", ";
        return s.substr(0, s.size() - 2) + "}";
    }

    // The first pointer in the list is a pointer to itself in attValPairs
    RadixTree<std::list<AttValPair *>> attValTree;
};

#endif // Include guard
