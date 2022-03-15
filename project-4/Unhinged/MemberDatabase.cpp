#include <fstream>
#include <list>
#include <string>
#include <vector>

#include "MemberDatabase.h"
#include "PersonProfile.h"
#include "RadixTree.h"
#include "utility.h"

bool MemberDatabase::LoadDatabase(std::string filename) {
    std::ifstream in(filename);
    if (!in.is_open())
        return false;

    std::string name, email;
    int attValCount;
    while (std::getline(in, name) && std::getline(in, email) && in >> attValCount) {
        if (profileTree.search(email))
            return false;

        profileTree.insert(email, SharedPersonProfile(*this, name, email));
        SharedPersonProfile *profile = profileTree.search(email);
        profile->resetTree();

        std::getline(in, name);
        std::string line, attribute, value;
        for (int i = 0; i < attValCount; i++) {
            std::getline(in, line);
            int comma = line.find(',');
            if (comma == std::string::npos)
                return false;
            attribute = line.substr(0, comma);
            value = line.substr(comma + 1);
            profile->AddAttValPair({attribute, value});
        }
        std::getline(in, name);
    }

    return true;
}

const PersonProfile *MemberDatabase::GetMemberByEmail(std::string email) const {
    return profileTree.search(email);
}

std::vector<std::string> MemberDatabase::FindMatchingMembers(const AttValPair &input) const {
    std::vector<std::string> results;
    std::string key = attValToString(input);
    AttValMemberList *attValList = attValListTree.search(key);
    if (!attValList)
        return results;

    for (PersonProfile *profile : attValList->members)
        results.push_back(profile->GetEmail());

    return results;
}

MemberDatabase::SharedPersonProfile::SharedPersonProfile(MemberDatabase &database, std::string name, std::string email)
    : PersonProfile(name, email), database(database) {}

void MemberDatabase::SharedPersonProfile::AddAttValPair(const AttValPair &attval) {
    std::string key = attValToString(attval);
    char *exists = attValTree.search(key);
    if (!exists) {
        AttValMemberList *attValList = database.attValListTree.search(key);
        if (!attValList) { // Database does not yet contain this AttValPair
            database.attValListTree.insert(key, {attval});
            attValList = database.attValListTree.search(key);
        }
        attValPairs.push_back(&attValList->attval);
        attValTree.insert(key, 'x');
        attValList->members.push_back(this);
    }
}

int MemberDatabase::SharedPersonProfile::GetNumAttValPairs() const {
    return attValPairs.size();
}

bool MemberDatabase::SharedPersonProfile::GetAttVal(int attribute_num, AttValPair &attval) const {
    if (attribute_num < 0 || attribute_num >= attValPairs.size())
        return false;

    attval = *attValPairs[attribute_num];
    return true;
}

void MemberDatabase::print() {
    std::cout << "profileTree" << std::endl;
    profileTree.print(SharedPersonProfile::toString);
    std::cout << std::endl << "attValListTree" << std::endl;
    attValListTree.print(AttValMemberList::toString);
}
