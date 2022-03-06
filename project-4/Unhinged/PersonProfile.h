#ifndef PERSONPROFILE_H
#define PERSONPROFILE_H

#include <string>
#include "provided.h"

class PersonProfile {
public:
    PersonProfile(std::string name, std::string email);
    ~PersonProfile();

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
    // One or more attribute-value pairs
};

#endif // Include guard
