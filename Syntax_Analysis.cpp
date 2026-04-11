#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>

using namespace std;

struct Production {
    string lhs;
    vector<string> rhs;
};

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
};

extern vector<Production> grammar;
extern set<string> terminals;
extern set<string> nonTerminals;

map<string, set<string>> FIRST;
map<string, set<string>> FOLLOW;

set<string> firstOfSequence(const vector<string>& seq) {
    set<string> result;
    bool epsilon = true;

    for (auto &X : seq) {
        epsilon = false;

        if (terminals.count(X)) {
            result.insert(X);
            return result;
        }

        for (auto &f : FIRST[X]) {
            if (f != "ε") result.insert(f);
        }

        if (!FIRST[X].count("ε")) return result;
        epsilon = true;
    }

    if (epsilon) result.insert("ε");
    return result;
}

void computeFIRST() {
    bool changed = true;

    while (changed) {
        changed = false;

        for (auto &prod : grammar) {
            string A = prod.lhs;
            auto &rhs = prod.rhs;

            if (rhs.empty()) {
                if (FIRST[A].insert("ε").second) changed = true;
                continue;
            }

            bool epsilon = true;

            for (auto &X : rhs) {
                epsilon = false;

                if (terminals.count(X)) {
                    if (FIRST[A].insert(X).second) changed = true;
                    break;
                }

                for (auto &f : FIRST[X]) {
                    if (f != "ε") {
                        if (FIRST[A].insert(f).second) changed = true;
                    }
                }

                if (!FIRST[X].count("ε")) break;
                epsilon = true;
            }

            if (epsilon) {
                if (FIRST[A].insert("ε").second) changed = true;
            }
        }
    }
}

void computeFOLLOW() {
    FOLLOW["S'"].insert("$");

    bool changed = true;

    while (changed) {
        changed = false;

        for (auto &prod : grammar) {
            string A = prod.lhs;
            auto &rhs = prod.rhs;

            for (int i = 0; i < rhs.size(); i++) {
                string B = rhs[i];
                if (!nonTerminals.count(B)) continue;

                if (i + 1 < rhs.size()) {
                    vector<string> beta(rhs.begin() + i + 1, rhs.end());
                    set<string> firstBeta = firstOfSequence(beta);

                    for (auto &f : firstBeta) {
                        if (f != "ε") {
                            if (FOLLOW[B].insert(f).second) changed = true;
                        }
                    }

                    if (firstBeta.count("ε")) {
                        for (auto &f : FOLLOW[A]) {
                            if (FOLLOW[B].insert(f).second) changed = true;
                        }
                    }
                } else {
                    for (auto &f : FOLLOW[A]) {
                        if (FOLLOW[B].insert(f).second) changed = true;
                    }
                }
            }
        }
    }
}

void printFIRST() {
    for (auto &p : FIRST) {
        cout << "FIRST(" << p.first << ") = { ";
        for (auto &x : p.second) cout << x << " ";
        cout << "}\n";
    }
}

void printFOLLOW() {
    for (auto &p : FOLLOW) {
        cout << "FOLLOW(" << p.first << ") = { ";
        for (auto &x : p.second) cout << x << " ";
        cout << "}\n";
    }
}

int main() {
    computeFIRST();
    computeFOLLOW();

    printFIRST();
    printFOLLOW();

    return 0;
}