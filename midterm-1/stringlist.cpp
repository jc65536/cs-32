#include <cassert>
#include <iostream>
#include <string>
using namespace std;

class StringList {
public:
    StringList(string text);
    // You may declare other member functions for your test code to call
    void eraseThrees();
    void print() {
        for (Node *n = head; n; n = n->next) {
            cout << n->value;
        }
        cout << endl;
    }
private:
    struct Node {
        char value;
        Node *next;
        Node(char v, Node *n) : value(v), next(n) {}
    };
    Node *head; // points to first Node in the list
};

StringList::StringList(string text) {
    //  For example, StringList s("heap"); creates s containing
    //  a list with values  'h'  'e'  'a'  'p'
    head = nullptr;
    for (int k = text.size(); k > 0; k--)
        head = new Node(text[k - 1], head);
}

// You may implement other member functions here for test purposes,
// but you will not turn them in as part of your submission.

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT AFTER THE END OF THE eraseThrees IMPLEMENTATION

void StringList::eraseThrees() {
    int i = 1;
    Node *prev = nullptr, *curr = head;
    while (curr) {
        if (i == 0) {
            prev->next = curr->next;
            delete curr;
            curr = prev->next;
        } else {
            prev = curr;
            curr = curr->next;
        }
        i = (i + 1) % 3;
    }
}

// SUBMIT AS YOUR ANSWER ONLY THE CODE BETWEEN THIS COMMENT AND THE
// COMMENT BEFORE THE START OF THE eraseThrees IMPLEMENTATION

int main() {
    StringList x("");
    x.eraseThrees();
    x.print();
    // You might put some code here for test purposes, e.g., to verify
    // that x now contains  't'  'r'  'a'  'c'  'e'  'r'
    // You might run some other tests.
    cout << "Program finished" << endl;
}
