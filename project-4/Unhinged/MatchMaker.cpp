#include <unordered_set>
#include <string>
#include <vector>

#include "AttributeTranslator.h"
#include "MatchMaker.h"
#include "MemberDatabase.h"
#include "PersonProfile.h"
#include "provided.h"

struct AttValHash {
    bool operator()(const AttValPair &attval) const {
        return std::hash<std::string>()(attValToString(attval));
    }
};

bool compEmailCount(const EmailCount &a, const EmailCount &b) {
    return a.count > b.count || a.count == b.count && a.email < b.email;
}

MatchMaker::MatchMaker(const MemberDatabase &mdb, const AttributeTranslator &at)
    : database(mdb), translator(at) {
}

std::vector<EmailCount> MatchMaker::IdentifyRankedMatches(std::string email, int threshold) const {
    std::vector<EmailCount> results;
    const PersonProfile *profile = database.GetMemberByEmail(email);
    if (!profile)
        return results;

    std::unordered_set<AttValPair, AttValHash> allCompat;

    int numAttValPairs = profile->GetNumAttValPairs();
    AttValPair attval;
    for (int i = 0; i < numAttValPairs; i++) {
        profile->GetAttVal(i, attval);
        std::vector<AttValPair> compat = translator.FindCompatibleAttValPairs(attval);
        allCompat.insert(compat.begin(), compat.end());
    }

    RadixTree<int> matchCounts;
    std::list<std::string> matchEmails;
    for (const AttValPair &attval : allCompat) {
        std::vector<std::string> matches = database.FindMatchingMembers(attval);
        for (std::string email : matches) {
            int *count = matchCounts.search(email);
            if (count) {
                (*count)++;
            } else {
                matchEmails.push_back(email);
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

    std::sort(results.begin(), results.end(), compEmailCount);

    return results;
}
