#include <bits/stdc++.h>
using namespace std;

struct Token {
    string type;
    string value;
};

struct Production {
    string lhs;
    vector<string> rhs;
};

vector<Production> grammar = {
    {"S'", {"Program"}},
    {"Program", {"StmtList"}},
    {"StmtList", {"Stmt", "StmtList"}},
    {"StmtList", {}},
    {"Stmt", {"DeclStmt"}},
    {"Stmt", {"AssignStmt"}},
    {"Stmt", {"WhileStmt"}},
    {"DeclStmt", {"int", "id", "[", "num", "]", ";"}},
    {"AssignStmt", {"id", "=", "Expr", ";"}},
    {"AssignStmt", {"id", "[", "Expr", "]", "=", "Expr", ";"}},
    {"WhileStmt", {"while", "(", "Cond", ")", "{", "StmtList", "}"}},
    {"Cond", {"Expr", "RelOp", "Expr"}},
    {"RelOp", {"<"}},
    {"RelOp", {">"}},
    {"RelOp", {"<="}},
    {"RelOp", {">="}},
    {"RelOp", {"=="}},
    {"RelOp", {"!="}},
    {"Expr", {"Expr", "+", "Term"}},
    {"Expr", {"Expr", "-", "Term"}},
    {"Expr", {"Term"}},
    {"Term", {"Term", "*", "Factor"}},
    {"Term", {"Term", "/", "Factor"}},
    {"Term", {"Factor"}},
    {"Factor", {"id"}},
    {"Factor", {"num"}},
    {"Factor", {"id", "[", "Expr", "]"}},
    {"Factor", {"(", "Expr", ")"}}
};

set<string> terminals = {
    "INT","WHILE","ID","NUM",
    "PLUS","MINUS","MUL","DIV","ASSIGN",
    "LT","GT","LE","GE","EQ","NE",
    "LBRACKET","RBRACKET","LPAREN","RPAREN",
    "LBRACE","RBRACE","SEMICOLON","$"
};

set<string> nonTerminals = {
    "Program","StmtList","Stmt",
    "DeclStmt","AssignStmt","WhileStmt",
    "Cond","RelOp","Expr","Term","Factor"
};

vector<Token> lexicalAnalysis(const string& src) {
    vector<Token> tokens;
    int n = src.size();

    for (int i = 0; i < n;) {

        if (isspace(src[i])) {
            i++;
            continue;
        }

        if (src[i] == '/' && i + 1 < n) {
            if (src[i + 1] == '/') {
                i += 2;
                while (i < n && src[i] != '\n') i++;
                continue;
            }
            if (src[i + 1] == '*') {
                i += 2;
                bool closed = false;
                while (i + 1 < n) {
                    if (src[i] == '*' && src[i + 1] == '/') {
                        i += 2;
                        closed = true;
                        break;
                    }
                    i++;
                }
                if (!closed) {
                    cout << "Lexical Error: Unclosed comment\n";
                    exit(1);
                }
                continue;
            }
        }

        if (isalpha(src[i])) {
            string word;
            while (i < n && (isalnum(src[i]) || src[i] == '_')) {
                word += src[i++];
            }

            if (word == "int") tokens.push_back({"INT", word});
            else if (word == "while") tokens.push_back({"WHILE", word});
            else tokens.push_back({"ID", word});
            continue;
        }

        if (isdigit(src[i])) {
            string num;
            while (i < n && isdigit(src[i])) {
                num += src[i++];    
            }
            tokens.push_back({"NUM", num});
            continue;
        }

        if (i + 1 < n) {
            string two = src.substr(i, 2);
            if (two == "<=") { tokens.push_back({"LE", two}); i += 2; continue; }
            if (two == ">=") { tokens.push_back({"GE", two}); i += 2; continue; }
            if (two == "==") { tokens.push_back({"EQ", two}); i += 2; continue; }
            if (two == "!=") { tokens.push_back({"NE", two}); i += 2; continue; }
        }

        char c = src[i];

        if (c == '+') tokens.push_back({"PLUS", "+"});
        else if (c == '-') tokens.push_back({"MINUS", "-"});
        else if (c == '*') tokens.push_back({"MUL", "*"});
        else if (c == '/') tokens.push_back({"DIV", "/"});
        else if (c == '=') tokens.push_back({"ASSIGN", "="});
        else if (c == '<') tokens.push_back({"LT", "<"});
        else if (c == '>') tokens.push_back({"GT", ">"});
        else if (c == '[') tokens.push_back({"LBRACKET", "["});
        else if (c == ']') tokens.push_back({"RBRACKET", "]"});
        else if (c == '(') tokens.push_back({"LPAREN", "("});
        else if (c == ')') tokens.push_back({"RPAREN", ")"});
        else if (c == '{') tokens.push_back({"LBRACE", "{"});
        else if (c == '}') tokens.push_back({"RBRACE", "}"});
        else if (c == ';') tokens.push_back({"SEMICOLON", ";"});
        else {
            cout << "Lexical Error: Invalid character '" << c << "'\n";
            exit(1);
        }

        i++;
    }

    tokens.push_back({"$", "$"});
    return tokens;
}

int main() {
    string src =
        "int arr[5];"
        "int x;"
        "arr[0]=10;"
        "x=arr[0]+2;"
        "while(x<5){arr[x]=x*2;}";

    vector<Token> tokens = lexicalAnalysis(src);

    for (auto &t : tokens) {
        cout << "(" << t.type << ", " << t.value << ")\n";
    }

    return 0;
}