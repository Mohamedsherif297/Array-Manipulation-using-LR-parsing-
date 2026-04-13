#include <iostream>
#include <vector>
#include <string>
#include "Lexer.h"
#include "Parser.h"

using namespace std;

int main() {

    string src = "int x; x = 5;";

    SymbolTable st;
    Lexer lexer(src, st);
    vector<Token> tokens = lexer.tokenize();

    computeFIRST();
    buildStates();
    buildParsingTable();

    vector<string> input;

    for (auto &t : tokens) {
        switch (t.getType()) {
            case TokenType::DATATYPE:
                input.push_back(t.getLexeme());
                break;
            case TokenType::IDENTIFIER:
                input.push_back("ID");
                break;
            case TokenType::CONSTANT:
                input.push_back("NUM");
                break;
            case TokenType::STRING:
                input.push_back("STRING");
                break;
            case TokenType::CHAR:
                input.push_back("CHAR");
                break;
            case TokenType::END_OF_FILE:
                input.push_back("$");
                break;
            default:
                input.push_back(t.getLexeme());
                break;
        }
    }

    Node* root = parse(input);

    if (root) {
        cout << "\nAST (JSON):\n";
        printJSON(root);
        cout << endl;
    }
    printParsingTable();


    return 0;
}