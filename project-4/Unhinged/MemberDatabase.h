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

    // REMOVE BEFORE SUBMISSION!!
    void print();

private:
    // Reuse AttValPair objects
    class SharedPersonProfile : public PersonProfile {
    public:
        SharedPersonProfile(MemberDatabase &database, std::string name, std::string email);
        void AddAttValPair(const AttValPair &attval) override;
        int GetNumAttValPairs() const override;
        bool GetAttVal(int attribute_num, AttValPair &attval) const override;

        // Resets the root pointer in attValTree after being copied into profileTree
        void resetTree() {
            attValTree = RadixTree<char>();
        }

        static std::string toString(const SharedPersonProfile &p) {
            std::string ret = p.GetName() + " " + p.GetEmail();
            for (AttValPair *a : p.attValPairs)
                ret += ", " + attValToString(*a);
            return ret;
        }

    private:
        std::vector<AttValPair *> attValPairs;
        RadixTree<char> attValTree;
        MemberDatabase &database;
    };

    struct AttValMemberList {
        AttValPair attval;
        std::list<SharedPersonProfile *> members;
        static std::string toString(const AttValMemberList &a) {
            std::string ret = attValToString(a.attval);
            for (SharedPersonProfile *p : a.members)
                ret += ", " + p->GetName();
            return ret;
        }
    };

    RadixTree<SharedPersonProfile> profileTree;
    RadixTree<AttValMemberList> attValListTree;
};

#endif // Include guard
