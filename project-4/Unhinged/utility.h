#ifndef UTILITY_H
#define UTILITY_H

#include <string>

#include "provided.h"

#define NUM_CHARS 128

std::string attValToString(AttValPair attval);

std::string listToString(const std::list<AttValPair *> &list);

#endif // Include guard
