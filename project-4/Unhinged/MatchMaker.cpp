#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "AttributeTranslator.h"
#include "MatchMaker.h"
#include "MemberDatabase.h"
#include "PersonProfile.h"
#include "provided.h"

struct AttValComp {
    bool operator()(const AttValPair &a, const AttValPair &b) const {
        return attValToString(a) < attValToString(b);
    }
};

MatchMaker::MatchMaker(const MemberDatabase &mdb, const AttributeTranslator &at)
    : database(mdb), translator(at) {
}

std::vector<EmailCount> MatchMaker::IdentifyRankedMatches(std::string email, int threshold) const {
    std::vector<EmailCount> results;
    const PersonProfile *profile = database.GetMemberByEmail(email);
    if (!profile)
        return results;

    std::set<AttValPair, AttValComp> allCompat;

    int numAttValPairs = profile->GetNumAttValPairs();
    AttValPair attval;
    for (int i = 0; i < numAttValPairs; i++) {
        profile->GetAttVal(i, attval);
        std::vector<AttValPair> compat = translator.FindCompatibleAttValPairs(attval);
        allCompat.insert(compat.begin(), compat.end());
    }

    RadixTree<int> matchCounts;
    std::set<std::string> matchEmails;
    for (const AttValPair &attval : allCompat) {
        std::vector<std::string> matches = database.FindMatchingMembers(attval);
        for (std::string email : matches) {
            int *count = matchCounts.search(email);
            if (count) {
                (*count)++;
            } else {
                matchEmails.insert(email);
                matchCounts.insert(email, 1);
            }
        }
    }

    for (std::string email : matchEmails) {
        int *count = matchCounts.search(email);
        if (*count >= threshold) {
            results.push_back({email, *count});
        }
    }

    return results;
}
