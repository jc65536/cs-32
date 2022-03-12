#ifndef PERSONPROFILE_H
#define PERSONPROFILE_H

#include <string>
#include <list>

#include "provided.h"
#include "RadixTree.h"

class PersonProfile {
public:
    PersonProfile(std::string name, std::string email);

    std::string GetName() const {
        return name;
    }

    std::string GetEmail() const {
        return email;
    }

    void AddAttValPair(const AttValPair &attval);
    int GetNumAttValPairs() const;
    bool GetAttVal(int attribute_num, AttValPair &attval) const;

private:
    std::string name;
    std::string email;
    std::vector<AttValPair> attValPairs;
    RadixTree<char> attValTree;
};

#endif // Include guard
