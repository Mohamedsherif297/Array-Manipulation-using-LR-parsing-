#include <vector>
#include <iostream>
#include <string>
#include <set>
using namespace std;

struct Token {
    string type;
    string value;
};

vector<Token> lexicalAnalysis(const string& src) {
    vector<Token> tokens;
    int n = src.size();

    for (int i = 0; i < n;) {

        // 🔹 Skip spaces
        if (isspace(src[i])) {
            i++;
            continue;
        }

        // 🔹 Single-line comment //
        if (src[i] == '/' && i + 1 < n && src[i + 1] == '/') {
            i += 2;
            while (i < n && src[i] != '\n') i++;
            continue;
        }

        // 🔹 Multi-line comment /* */
        if (src[i] == '/' && i + 1 < n && src[i + 1] == '*') {
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

        // 🔹 STRING "..."
        if (src[i] == '\"') {
            i++;
            string str;

            while (i < n && src[i] != '\"') {
                str += src[i++];
            }

            if (i == n) {
                cout << "Lexical Error: Unclosed string\n";
                exit(1);
            }

            i++; // skip closing "
            tokens.push_back({"STRING", str});
            continue;
        }

        // 🔹 CHAR 'a'
        if (src[i] == '\'') {
            i++;

            if (i >= n || i + 1 >= n || src[i + 1] != '\'') {
                cout << "Lexical Error: Invalid char\n";
                exit(1);
            }

            string ch(1, src[i]);
            i += 2;

            tokens.push_back({"CHAR", ch});
            continue;
        }

        // 🔹 IDENTIFIER or KEYWORD
        if (isalpha(src[i]) || src[i] == '_') {
            string word;

            while (i < n && (isalnum(src[i]) || src[i] == '_')) {
                word += src[i++];
            }

            if (word == "int" || word == "float" || word == "double" || word == "char" || word == "string")
                tokens.push_back({"Type", word});  // matches grammar
            else
                tokens.push_back({"ID", word});

            continue;
        }

        // 🔹 NUMBER (int + float)
        if (isdigit(src[i])) {
            string num;
            bool hasDot = false;

            while (i < n && (isdigit(src[i]) || src[i] == '.')) {
                if (src[i] == '.') {
                    if (hasDot) break;
                    hasDot = true;
                }
                num += src[i++];
            }

            tokens.push_back({"NUM", num});
            continue;
        }

        // 🔹 OPERATORS & SYMBOLS
        char c = src[i];

        if (c == '+') tokens.push_back({"OPERATOR", "+"});

        else if (c == '-') tokens.push_back({"OPERATOR", "-"});

        else if (c == '*') tokens.push_back({"OPERATOR", "*"});

        else if (c == '/') tokens.push_back({"OPERATOR", "/"});

        else if (c == '=') tokens.push_back({"OPERATOR", "="});

        else if (c == '[') tokens.push_back({"LBRACKET", "["});

        else if (c == ']') tokens.push_back({"RBRACKET", "]"});

        else if (c == '(') tokens.push_back({"LPAREN", "("});

        else if (c == ')') tokens.push_back({"RPAREN", ")"});

        else if (c == '{') tokens.push_back({"LBRACE", "{"});

        else if (c == '}') tokens.push_back({"RBRACE", "}"});

        else if (c == ',') tokens.push_back({"SYMBOL", ","});

        else if (c == ';') tokens.push_back({"SYMBOL", ";"});

        else if (c == '\"') tokens.push_back({"SYMBOL", "\""});

        else if (c == '\'') tokens.push_back({"SYMBOL", "\'"}); 

        else {

        cout << "Lexical Error: Invalid character '" << c << "'\n";
            exit(1);
        }

        i++;
    }

    tokens.push_back({"$", "$"}); // End marker
    return tokens;
}


int main() {
    string src =
        "string x = \"\";"
        "string x = \"     \"; ";

    vector<Token> tokens = lexicalAnalysis(src);

    for (auto &t : tokens) {
        cout << "(" << t.type << ", " << t.value << ")\n";
    }

    return 0;
}
    