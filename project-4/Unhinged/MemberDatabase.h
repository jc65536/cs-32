#ifndef MEMBERDATABASE_H
#define MEMBERDATABASE_H

#include <list>
#include <string>

#include "PersonProfile.h"
#include "RadixTree.h"
#include "provided.h"

class MemberDatabase {
public:
    bool LoadDatabase(std::string filename);
    const PersonProfile *GetMemberByEmail(std::string email) const;
    std::vector<std::string> FindMatchingMembers(const AttValPair &input) const;

private:
    // Save space by using pointers instead of copying strings
    class SharedPersonProfile : public PersonProfile {
    public:
        SharedPersonProfile(MemberDatabase &database, std::string name, std::string email);
        void AddAttValPair(const AttValPair &attval) override;
        int GetNumAttValPairs() const override;
        bool GetAttVal(int attribute_num, AttValPair &attval) const override;

        // Creates a new attValTree after being copied into profileTree
        void createTree() {
            attValTree = new RadixTree<char>();
        }

        // Delete attValTree to save memory
        void deleteTree() {
            delete attValTree;
        }

    private:
        std::vector<AttValPair *> attValPairs;
        RadixTree<char> *attValTree;
        MemberDatabase &database;
    };

    // Associates each AttValPair with a list of members
    struct AttValMemberList {
        AttValPair attval;
        std::list<SharedPersonProfile *> members;
    };

    RadixTree<SharedPersonProfile> profileTree;
    RadixTree<AttValMemberList> attValListTree;
};

#endif // Include guard
