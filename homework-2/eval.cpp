#include "Map.h"
#include <cassert>
#include <cctype>
#include <iostream>
#include <stack>
#include <string>
using namespace std;

string clean(const string &str) {
    string newstr = "";
    for (char c : str) {
        if (c != ' ')
            newstr += c;
    }
    return newstr;
}

int inToPost(string infix, string &postfix) {
    //cerr << infix << endl;
    postfix = "";
    stack<char> st;
    char last = 0;
    for (char c : infix) {
        if (islower(c)) {
            if (islower(last) || last == ')') {
                //cerr << "Illegal character sequence: " << last << c << endl;
                return 1;
            }
            postfix += c;
        } else if (c == '(') {
            if (islower(last) || last == ')') {
                //cerr << "Illegal character sequence: " << last << c << endl;
                return 1;
            }
            st.push(c);
        } else if (c == ')') {
            if (last == '+' || last == '(') {
                //cerr << "Illegal character sequence: " << last << c << endl;
                return 1;
            }
            while (!st.empty() && st.top() != '(') {
                postfix += st.top();
                st.pop();
            }
            if (!st.empty()) {
                st.pop();
            } else {
                //cerr << "Missing opening parenthesis" << endl;
                return 1;
            }
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (last == '+' || last == '(') {
                //cerr << "Illegal character sequence: " << last << c << endl;
                return 1;
            }
            char top;
            while (!st.empty() && (top = st.top()) != '(' && (c == '+' || c == '-' || top == '*' || top == '/')) {
                postfix += top;
                st.pop();
            }
            st.push(c);
            c = '+';
        } else {
            //cerr << "Illegal character: " << c << endl;
            return 1;
        }
        last = c;
    }

    while (!st.empty()) {
        if (st.top() == '(') {
            //cerr << "Unclosed parenthesis" << endl;
            return 1;
        }
        postfix += st.top();
        st.pop();
    }

    //cerr << "Successfully translated postfix: " << postfix << endl;
    return 0;
}

int postToNum(string postfix, const Map &values, int &result) {
    stack<int> st;
    for (char c : postfix) {
        if (islower(c)) {
            int val;
            if (!values.get(c, val)) {
                //cerr << "Unknown variable: " << c << endl;
                return 2;
            }
            st.push(val);
        } else {
            if (st.size() < 2) {
                //cerr << "Missing operand for " << c << endl;
                return 1;
            }
            int o2 = st.top();
            st.pop();
            int o1 = st.top();
            st.pop();

            switch (c) {
            case '+':
                st.push(o1 + o2);
                break;
            case '-':
                st.push(o1 - o2);
                break;
            case '*':
                st.push(o1 * o2);
                break;
            case '/':
                if (o2 == 0) {
                    //cerr << "Division by 0" << endl;
                    return 3;
                }
                st.push(o1 / o2);
                break;
            }
        }
    }

    if (st.size() != 1) {
        //cerr << st.size() - 1 << " extraneous operands" << endl;
        return 1;
    }

    result = st.top();
    //cerr << "Evaluated result: " << result << endl;
    return 0;
}

int evaluate(string infix, const Map &values, string &postfix, int &result) {
    return inToPost(clean(infix), postfix) ? 1 : postToNum(postfix, values, result);
}
