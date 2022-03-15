#include <string>
#include <list>

#include "utility.h"
#include "provided.h"

std::string attValToString(AttValPair attval) {
    return attval.attribute + "\0" + attval.value;
}
