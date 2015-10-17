#include "qac/parser/parser.h"

/*
 * The grammar
 * ===========
 *
 * ROOT                            ::= QUESTION+ 
 *                                   | CHAPTER+
 *
 * QUESTION                        ::= TOKEN_QUESTION QUESTION_TEXT TOKEN_ANSWER ANSWER_TEXT
 *
 * QUESTION_TEXT ::=
 *
 * ANSWER_TEXT                     ::= VALID_ANSWER_TEXT+
 *
 * VALID_ANSWER_TEXT               ::= 
 *
 * TEXT                            ::= TOKEN_WORD+
 *
 * LATEX                           ::= NORMAL_LATEX 
 *                                   | CENTERED_LATEX
 *
 * NORMAL_LATEX                    ::= TOKEN_LATEX_OPENING LATEX_BODY TOKEN_LATEX_CLOSING
 *
 * CENTERED_LATEX                  ::= TOKEN_LATEX_CENTERED_OPENING LATEX_BODY TOKEN_LATEX_CENTERED_CLOSING
 *
 * LATEX_BODY                      ::= TOKEN_LATEX_CODE+
 *
 * UNORDERED_LIST                  ::= UNORDERED_LIST_ITEM+
 *
 * UNORDERED_LIST_ITEM             ::= TOKEN_UNORDERED_LIST_ITEM LIST_ITEM_TEXT
 *
 * ORDERED_LIST                    ::= ORDERED_LIST_ITEM+
 *
 * ORDERED_LIST_ITEM               ::= TOKEN_ORDERED_LIST_ITEM LIST_ITEM_TEXT
 *
 * LIST_ITEM_TEXT                  ::=
 *
 * BOLD                            ::= TOKEN_BOLD_OPENING TEXT TOKEN_BOLD_CLOSING
 *
 * UNDERLINED                      ::= TOKEN_UNDERLINE_OPENING TEXT TOKEN_UNDERLINE_CLOSING
 *
 * CODE                            ::= TOKEN_CODE_OPENING TEXT TOKEN_CODE_CLOSING
 *
 * CHAPTER                         ::= TOKEN_CHAPTER TEXT QUESTION* SECTION*
 *
 * SECTION                         ::= TOKEN_SECTION TEXT QUESTION* SUBSECTION*
 *
 * SUBSECTION                      ::= TOKEN_SUBSECTION TEXT QUESTION*
 *
 * TABLE                           ::= TOKEN_TABLE_DIVIDER TABLE_ROW+
 *
 * TABLE_ROW                       ::= TABLE_CELL+ TOKEN_TABLE_DIVIDER
 *
 * TABLE_CELL                      ::= TOKEN_TABLE_CELL 
 *                                   | TOKEN_TABLE_CELL_LEFT_ALIGNED 
 *                                   | TOKEN_TABLE_CELL_RIGHT_ALIGNED 
 *                                   | TOKEN_TABLE_CELL_CENTER_ALIGNED
 *
 */

using namespace qac;
using namespace std;

unique_ptr<node> parser::parse(const std::vector<token> &tokens) {
    unique_ptr<node> root = make_unique<node>(node_enum::ROOT);

    for (vector<token>::const_iterator it = cbegin(tokens); it != cend(tokens);
         ++it) {
        root->add_child(parse(tokens, it));
    }

    return root;
}

unique_ptr<node> parser::parse(const vector<token> &tokens,
                               vector<token>::const_iterator &current) {
    return make_unique<node>(node_enum::UNDERLINED);
}
