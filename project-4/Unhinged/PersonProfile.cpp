#include <string>

#include "PersonProfile.h"

PersonProfile::PersonProfile(std::string name, std::string email)
    : name(name), email(email) {}

void PersonProfile::AddAttValPair(const AttValPair &attval) {
    const std::string key = attval.attribute + "\0" + attval.value;
    char *exists = attValTree.search(key);
    if (!exists) {
        attValPairs.push_back(attval);
        attValTree.insert(key, 'x');
    }
}

int PersonProfile::GetNumAttValPairs() const {
    return attValPairs.size();
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair &attval) const {
    if (attribute_num < 0 || attribute_num >= attValPairs.size())
        return false;

    attval = attValPairs[attribute_num];
    return true;
}
