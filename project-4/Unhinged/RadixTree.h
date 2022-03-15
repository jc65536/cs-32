#ifndef RADIXTREE_H
#define RADIXTREE_H

#include <algorithm>
#include <iostream>
#include <list>
#include <string>

#include "utility.h"

template <class ValueType>
class RadixTree {
public:
    RadixTree() {
        root = newNode("ROOT");
    }

    void insert(std::string key, const ValueType &value) {
        Node *n = root;
        int status = search(key, n);

        if (status == 0) {
            delete n->value;
        } else {
            Node *&next = n->children[(int) key[0]];
            if (status == -1) { // Just create a new node
                n = newNode(key);
                next = n;
            } else { // Create a factor node for the new node and the existing node
                Node *factor = newNode(key.substr(0, status));
                key = key.substr(status);
                std::string &nextSegment = next->segment;
                nextSegment = nextSegment.substr(status);

                if (key.empty()) {
                    n = factor;
                } else {
                    n = newNode(key);
                    factor->children[(int) key[0]] = n;
                }

                factor->children[(int) nextSegment[0]] = next;
                next = factor;
            }
        }

        n->value = new ValueType(value);
    }

    ValueType *search(std::string key) const {
        Node *n = root;
        if (search(key, n) == 0) {
            return n->value;
        } else {
            return nullptr;
        }
    }

private:
    struct Node {
        std::string segment;
        Node *children[NUM_CHARS];
        ValueType *value;

        Node(std::string segment)
            : segment(segment),
              children{nullptr},
              value(nullptr) {}

        ~Node() {
            delete value;
        }
    } * root;

    /*
    Starting from n, searches the radix tree as much as possible
    Returns a status code:
        0   Found an exact match
        -1  No need to factor
        n   n is the length of the common factor
    After the function returns, n will point to the "closest possible" node, and
    key will contain the rest of the key if an exact match couldn't be found.
    */
    int search(std::string &key, Node *&n) const {
        if (!n) {
            std::cerr << "n is nullptr!" << std::endl;
            exit(1);
        }

        while (!key.empty()) {
            Node *next = n->children[(int) key[0]];
            if (!next)
                return -1;

            std::string nextSegment = next->segment;
            int keySize = key.size();
            int nextSize = nextSegment.size();
            int minSize = std::min(keySize, nextSize);

            // Check if key and nextSegment have any conflicting characters
            for (int i = 1; i < minSize; i++) {
                if (key[i] != nextSegment[i])
                    return i;
            }

            if (keySize < nextSize) {
                // key is a prefix of nextSegment
                return keySize;
            } else {
                // nextSegment is a prefix of or equal to key
                key = key.substr(nextSize);
                n = next;
            }
        }

        return 0;
    }

    Node *newNode(std::string segment) {
        nodes.push_back(Node(segment));
        return &nodes.back();
    }

    std::list<Node> nodes;
};

#endif // Include guard
