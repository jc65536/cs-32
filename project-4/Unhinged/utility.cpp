#include <string>
#include <list>

#include "utility.h"
#include "provided.h"

std::string attValToString(AttValPair attval) {
    return attval.attribute + ":\0" + attval.value;
}

std::string listToString(const std::list<AttValPair *> &list) {
    std::string s = "{";
    for (AttValPair *p : list)
        s += attValToString(*p) + ", ";
    return s.substr(0, s.size() - 2) + "}";
}
