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
}
