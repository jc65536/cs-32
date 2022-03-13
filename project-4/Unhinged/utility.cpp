#include <string>
#include <list>

#include "utility.h"
#include "provided.h"

std::string attValToKey(AttValPair attval) {
    return attval.attribute + ":\0" + attval.value;
}

std::string listToString(const std::list<AttValPair *> &list) {
    std::string s = "{";
    for (AttValPair *p : list)
        s += attValToKey(*p) + ", ";
    return s.substr(0, s.size() - 2) + "}";
}
