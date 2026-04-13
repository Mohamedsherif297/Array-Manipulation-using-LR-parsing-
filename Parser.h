#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
#include "Parsing_Table.cpp"

using namespace std;

struct Node {
    string type;
    vector<Node*> children;
};

Node* parse(vector<string> input) {

    stack<int> stateStack;
    stack<Node*> nodeStack;

    stateStack.push(0);
    int index = 0;

    while (true) {

        int state = stateStack.top();
        string token = input[index];

        if (!ACTION.count({state, token})) {
            cout << "Syntax Error at token: " << token << endl;
            return nullptr;
        }

        string action = ACTION[{state, token}];

        // SHIFT
        if (action[0] == 'S') {
            int nextState = stoi(action.substr(1));

            Node* node = new Node();
            node->type = token;

            nodeStack.push(node);
            stateStack.push(nextState);

            index++;
        }

        // REDUCE
        else if (action[0] == 'R') {

            int prodIndex = stoi(action.substr(1));
            Production p = grammar[prodIndex];


            vector<Node*> children;

            for (int i = 0; i < p.rhs.size(); i++) {
                stateStack.pop();

                Node* child = nodeStack.top();
                nodeStack.pop();

                children.push_back(child);
            }

            reverse(children.begin(), children.end());
            newNode->children = children;

            int currentState = stateStack.top();

            if (!GOTO_TABLE.count({currentState, p.lhs})) {
                cout << "Goto Error\n";
                return nullptr;
            }

            int nextState = GOTO_TABLE[{currentState, p.lhs}];

            nodeStack.push(newNode);
            stateStack.push(nextState);
        }

        // ACCEPT
        else if (action == "ACC") {
            cout << "Parsing Successful\n";
            return nodeStack.top();
        }
    }
}
void printJSON(Node* node, int indent = 0) {

    if (!node) return;

    string space(indent * 2, ' ');

    cout << space << "{\n";
    cout << space << "  \"type\": \"" << node->type << "\"";

    if (!node->children.empty()) {
        cout << ",\n";
        cout << space << "  \"children\": [\n";

        for (int i = 0; i < node->children.size(); i++) {
            printJSON(node->children[i], indent + 4);

            if (i != node->children.size() - 1)
                cout << ",";

         cout << "\n";
        }

        cout << space << "  ]\n";
        cout << space << "}";
    }
    else {
        cout << "\n" << space << "}";
    }
}