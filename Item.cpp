#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "Grammar.cpp"
using namespace std;

struct Item {
    string lhs;
    vector<string> rhs;
    int dotPos;
    string lookahead;

    bool operator<(const Item& other) const {
        if (lhs != other.lhs) return lhs < other.lhs;
        if (rhs != other.rhs) return rhs < other.rhs;
        if (dotPos != other.dotPos) return dotPos < other.dotPos;
        return lookahead < other.lookahead;
    }

    bool operator==(const Item& other) const {
    return lhs == other.lhs &&
           rhs == other.rhs &&
           dotPos == other.dotPos &&
           lookahead == other.lookahead;
}

string nextSymbol() const {
    if (dotPos < rhs.size())
        return rhs[dotPos];
    return "";
    }

    bool isComplete() const {
    return dotPos >= rhs.size();
    }

    Item advanceDot() const {
    Item next = *this;
    next.dotPos++;
    return next;
    }

    void print() const {
    cout << lhs << " → ";
    for (int i = 0; i < rhs.size(); i++) {
        if (i == dotPos) cout << "• ";
        cout << rhs[i] << " ";
    }
    if (dotPos == rhs.size()) cout << "• ";
    cout << ", " << lookahead << endl;
 }
};

set<Item> closure(set<Item> I) {
    set<Item> result = I;
    bool changed = true;

    while (changed) {
        changed = false;

        vector<Item> items(result.begin(), result.end());

        for (auto &item : items) {
            string B = item.nextSymbol();

            if (B == "" || !nonTerminals.count(B)) continue;

            vector<string> beta;
            for (int i = item.dotPos + 1; i < item.rhs.size(); i++)
                beta.push_back(item.rhs[i]);

            beta.push_back(item.lookahead);

            set<string> lookaheads = firstOfSequence(beta);

            for (auto &prod : grammar) {
                if (prod.lhs != B) continue;

                for (auto &la : lookaheads) {
                    Item newItem;
                    newItem.lhs = B;
                    newItem.rhs = prod.rhs;
                    newItem.dotPos = 0;
                    newItem.lookahead = la;

                    if (result.insert(newItem).second)
                        changed = true;
                }
            }
        }
    }

    return result;
}

set<Item> GOTO(set<Item> I, string X) {
    set<Item> moved;

    for (auto &item : I) {
        if (item.nextSymbol() == X) {
            moved.insert(item.advanceDot());
        }
    }

    return closure(moved);
}