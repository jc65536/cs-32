#include <string>

#include "PersonProfile.h"

PersonProfile::PersonProfile(std::string name, std::string email)
    : name(name), email(email) {}

void PersonProfile::AddAttValPair(const AttValPair &attval) {
    auto attValPtrPtr = attValMapper.search(attval.attribute + attval.value);
    if (!attValPtrPtr) {
        attValPairs.push_back(attval);
        attValMapper.insert(&attValPairs.back());
    }
}

int PersonProfile::GetNumAttValPairs() const {
    return attValPairs.size();
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair &attval) const {
    std::string attribute = attributes[attribute_num];
    std::string *value = attValMapper.search(attribute);
    if (!value) {
        return false;
    } else {
        attval = AttValPair(attribute, *value);
        return true;
    }
}
