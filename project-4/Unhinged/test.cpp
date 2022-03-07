#include <vector>
#include <string>
#include <fstream>
#include "RadixTree.h"

int main() {
    RadixTree<int> radixTree;
    std::ifstream inp("test.txt");
    std::string line;
    int i = 0;
    while (inp >> line) {
        radixTree.insert(line, i);
        i++;
    }
    radixTree.print();

    std::cout << *radixTree.search("hello") << std::endl;
    std::cout << *radixTree.search("grate") << std::endl;
    std::cout << radixTree.search("pro") << std::endl;
    std::cout << radixTree.search("zzz") << std::endl;
}
