#ifndef MEMBERDATABASE_H
#define MEMBERDATABASE_H

#include <string>
#include <vector>
#include "provided.h"
#include "PersonProfile.h"

class MemberDatabase {
public:
    MemberDatabase();
    ~MemberDatabase();
    bool LoadDatabase(std::string filename);
    const PersonProfile *GetMemberByEmail(std::string email) const;
};

#endif // Include guard
