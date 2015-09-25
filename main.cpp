#include <iostream>
#include "lexer.h"

using namespace qac;
using namespace std;

#define TOKEN(t) case token_enum::t: return #t
std::string to_string(token_enum tenum) {
    switch (tenum) {
        TOKEN(QUESTION);
        TOKEN(ANSWER);
        TOKEN(WORD);
        TOKEN(LATEX_OPENING);
        TOKEN(LATEX_CENTERED_OPENING);
        TOKEN(LATEX_CODE);
        TOKEN(LATEX_CLOSING);
        TOKEN(LATEX_CENTERED_CLOSING);
        TOKEN(NEW_LINE);
        TOKEN(EMPTY_LINE);
        TOKEN(UNORDERED_LIST_ITEM);
        TOKEN(ORDERED_LIST_ITEM);
        TOKEN(TABLE_CELL);
        TOKEN(TABLE_CELL_LEFT_ALIGNED);
        TOKEN(TABLE_CELL_RIGHT_ALIGNED);
        TOKEN(TABLE_CELL_CENTER_ALIGNED);
        TOKEN(TABLE_DIVIDER);
        TOKEN(BOLD_OPENING);
        TOKEN(BOLD_CLOSING);
        TOKEN(UNDERLINE_OPENING);
        TOKEN(UNDERLINE_CLOSING);
    }
}
#undef TOKEN

int main(int argc, const char* argv[]) {
    if (argc < 3) {
        string exe(argv[0]);

        cerr << "Usage: " << exe.substr(exe.rfind('/') + 1) + " <generator> <qa-file> [<output>]\n";
        return -1;
    }

    try {
        lexer lexer;
        vector<token> tokens = lexer.lex(argv[2]);


        for(token token : tokens) {
            cout << "t: " << to_string(token.get_token()) << " " << token.get_value() << endl;
        }
    } catch (exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}