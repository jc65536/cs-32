#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "AttributeTranslator.h"
#include "provided.h"
#include "utility.h"

bool AttributeTranslator::Load(std::string filename) {
    std::ifstream in(filename);
    if (!in.is_open())
        return false;

    std::string line;
    std::string tokens[4];
    while (std::getline(in, line)) {
        size_t i = 0;
        int tokenCount = 0;
        while (i < line.size() && tokenCount < 4) {
            size_t j = line.find(',', i);
            if (j == std::string::npos)
                j = line.size();
            tokens[tokenCount] = line.substr(i, j - i);
            i = j + 1;
            tokenCount++;
        }

        // Check for malformed line
        if (i < line.size() || tokenCount < 4)
            return false;

        AttValPair source{tokens[0], tokens[1]},
            compat{tokens[2], tokens[3]};

        std::string sourceKey = attValToString(source),
                    compatKey = attValToString(compat);

        std::list<AttValPair *> *sourceList = attValTree.search(sourceKey);
        std::list<AttValPair *> *compatList = attValTree.search(compatKey);

        AttValPair *compatPtr; // Pointer to the compatible AttValPair in attValPairs

        if (compatList) { // compat already exists in attValPairs
            compatPtr = compatList->front();
        } else {
            attValPairs.push_back(compat);
            compatPtr = &attValPairs.back();
            attValTree.insert(compatKey, std::list<AttValPair *>{compatPtr});
        }

        if (sourceList) { // source already exists in attValPairs
            sourceList->push_back(compatPtr);
        } else {
            attValPairs.push_back(source);
            attValTree.insert(sourceKey, std::list<AttValPair *>{&attValPairs.back(), compatPtr});
        }
    }

    return true;
}

std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair &source) const {
    std::vector<AttValPair> result;
    std::list<AttValPair *> *list = attValTree.search(attValToString(source));
    if (!list)
        return result;
    
    for (auto it = ++list->begin(); it != list->end(); it++)
        result.push_back(**it);

    return result;
}

void AttributeTranslator::print() {
    attValTree.print(listToString);
}
