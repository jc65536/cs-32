#include <string>

#include "PersonProfile.h"
#include "provided.h"
#include "utility.h"

PersonProfile::PersonProfile(std::string name, std::string email)
    : name(name), email(email), attValTree(nullptr) {}

PersonProfile::~PersonProfile() {
    delete attValTree;
}

// Complexity: O(1)
void PersonProfile::AddAttValPair(const AttValPair &attval) {
    // Only initialize tree when needed to save memory
    if (!attValTree)
        attValTree = new RadixTree<char>();

    std::string key = attValToString(attval);
    char *exists = attValTree->search(key); // O(1)
    if (!exists) {
        attValPairs.push_back(attval); // O(1)
        attValTree->insert(key, 'x');   // O(1)
    }
}

int PersonProfile::GetNumAttValPairs() const {
    return attValPairs.size();
}

// Complexity: O(1)
bool PersonProfile::GetAttVal(int attribute_num, AttValPair &attval) const {
    if (attribute_num < 0 || attribute_num >= attValPairs.size())
        return false;
    attval = attValPairs[attribute_num];
    return true;
}
