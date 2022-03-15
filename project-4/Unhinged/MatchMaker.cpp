#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <string>
#include <vector>

#include "AttributeTranslator.h"
#include "MatchMaker.h"
#include "MemberDatabase.h"
#include "PersonProfile.h"
#include "provided.h"

struct AttValHasher {
    bool operator()(const AttValPair &attval) const {
        return std::hash<std::string>()(attValToString(attval));
    }
};

bool compEmailCount(const EmailCount &a, const EmailCount &b) {
    return a.count > b.count || (a.count == b.count && a.email < b.email);
}

MatchMaker::MatchMaker(const MemberDatabase &mdb, const AttributeTranslator &at)
    : database(mdb), translator(at) {
}

std::vector<EmailCount> MatchMaker::IdentifyRankedMatches(std::string email, int threshold) const {
    std::vector<EmailCount> results;
    const PersonProfile *profile = database.GetMemberByEmail(email);
    if (!profile)
        return results;

    std::unordered_set<AttValPair, AttValHasher> allCompat;

    int numAttValPairs = profile->GetNumAttValPairs();
    AttValPair attval;
    // O(N), where N is the number of AttValPairs
    for (int i = 0; i < numAttValPairs; i++) {
        profile->GetAttVal(i, attval); // O(1)
        std::vector<AttValPair> compat = translator.FindCompatibleAttValPairs(attval); // O(1)
        allCompat.insert(compat.begin(), compat.end()); // O(1)
    }

    std::unordered_map<std::string, int> emailCounts;
    // Overall: O(MN), where M is the number of matching members
    for (const AttValPair &attval : allCompat) { // O(N)
        std::vector<std::string> matches = database.FindMatchingMembers(attval); // O(M)
        for (std::string email : matches) // O(M)
            emailCounts[email]++;
    }

    // O(M)
    for (std::pair<const std::string, int> &emailCount : emailCounts) {
        if (emailCount.second >= threshold)
            results.push_back({emailCount.first, emailCount.second});
    }

    // O(M log M)
    std::sort(results.begin(), results.end(), compEmailCount);

    return results;
}
