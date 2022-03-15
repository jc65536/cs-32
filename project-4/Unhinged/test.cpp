#include <vector>
#include <string>
#include <fstream>
#include <cassert>

#include "RadixTree.h"
#include "PersonProfile.h"
#include "AttributeTranslator.h"
#include "MemberDatabase.h"
#include "MatchMaker.h"

#define TESTING_RADIXTREE           0
#define TESTING_PERSONPROFILE       0
#define TESTING_ATTRIBUTETRANSLATOR 0
#define TESTING_MEMBERDATABASE      0
#define TESTING_MATCHMAKER          1

int main() {
    #if TESTING_RADIXTREE
    {
        RadixTree<int> rt;
        std::ifstream in("test-radixtree.txt");
        std::string line;
        int i = 0;
        while (in >> line) {
            rt.insert(line, i);
            i++;
        }
        rt.print();

        std::cout << *rt.search("hello") << std::endl;
        std::cout << *rt.search("grate") << std::endl;
        std::cout << rt.search("pro") << std::endl;
        std::cout << rt.search("zzz") << std::endl;
    }
    #endif

    #if TESTING_PERSONPROFILE
    {
        PersonProfile p{"Jason", "jasonc224@ucla.edu"};
        std::cout << p.GetName() << " " << p.GetEmail() << std::endl;
        {
            AttValPair a("cats", "a");
            p.AddAttValPair(a);
        }
        p.AddAttValPair(AttValPair("p", "b"));
        p.AddAttValPair(AttValPair("par", "c"));
        p.AddAttValPair(AttValPair("parrot", "d"));
        p.AddAttValPair(AttValPair("party", "e"));
        p.AddAttValPair(AttValPair("cat", "f"));
        p.AddAttValPair(AttValPair("p", "g"));
        int n = p.GetNumAttValPairs();
        std::cout << n << std::endl;
        for (int i = 0; i < n; i++) {
            AttValPair a;
            p.GetAttVal(i, a);
            std::cout << a.attribute << "->" << a.value << std::endl;
        }
    }
    #endif

    #if TESTING_ATTRIBUTETRANSLATOR
    {
        AttributeTranslator at;
        at.Load("test-attributetranslator.txt");
        at.print();
        auto v = at.FindCompatibleAttValPairs(AttValPair("favorite_food", "del taco"));
        for (auto p : v) {
            std::cout << attValToString(p) << std::endl;
        }
        v = at.FindCompatibleAttValPairs(AttValPair("favorite_food", "mexican"));
        for (auto p : v) {
            std::cout << attValToString(p) << std::endl;
        }
    }
    #endif

    #if TESTING_MEMBERDATABASE
    {
        MemberDatabase md;
        assert(md.LoadDatabase("test-memberdatabase.txt"));
        md.print();

        const PersonProfile &p = *md.GetMemberByEmail("AbFow2483@charter.net");
        std::cout << std::endl << p.GetName() << " " << p.GetEmail() << std::endl;
        int n = p.GetNumAttValPairs();
        std::cout << n << std::endl;
        for (int i = 0; i < n; i++) {
            AttValPair a;
            p.GetAttVal(i, a);
            std::cout << a.attribute << "->" << a.value << std::endl;
        }
        std::cout << std::endl;

        std::vector<std::string> v = md.FindMatchingMembers({"hobby", "canyoneering"});
        for (std::string s : v) {
            std::cout << s << std::endl;
        }

        v = md.FindMatchingMembers({"hoxby", "canyoneering"});
        for (std::string s : v) {
            std::cout << s << std::endl;
        }
    }
    #endif

    #if TESTING_MATCHMAKER
    {
        MatchMaker mm;
    }
    #endif
}
