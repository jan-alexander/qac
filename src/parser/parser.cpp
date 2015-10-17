#include "qac/parser/parser.h"

/*
 * The grammar
 * ===========
 *
 * ROOT                            ::= QUESTION OPT_QUESTION
 *                                   | CHAPTER OPT_CHAPTER
 *
 * QUESTION                        ::= TOKEN_QUESTION QUESTION_TEXT TOKEN_ANSWER ANSWER_TEXT
 * 
 * OPT_QUESTION                    ::= QUESTION OPT_QUESTION
 *                                   | EMPTY_WORD
 *
 * QUESTION_TEXT                   ::= TEXT VALID_QUESTION_TEXT
 *                                   | LATEX VALID_QUESTION_TEXT
 *                                   | BOLD VALID_QUESTION_TEXT
 *                                   | UNDERLINED VALID_QUESTION_TEXT
 *                                   | CODE VALID_QUESTION_TEXT
 *
 * VALID_QUESTION_TEXT             ::= TEXT VALID_QUESTION_TEXT
 *                                   | LATEX VALID_QUESTION_TEXT
 *                                   | BOLD VALID_QUESTION_TEXT
 *                                   | UNDERLINED VALID_QUESTION_TEXT
 *                                   | CODE VALID_QUESTION_TEXT
 *                                   | EMPTY_WORD
 *
 * ANSWER_TEXT                     ::= TEXT VALID_ANSWER_TEXT
 *                                   | LATEX VALID_ANSWER_TEXT
 *                                   | UNORDERED_LIST VALID_ANSWER_TEXT
 *                                   | ORDERED_LIST VALID_ANSWER_TEXT
 *                                   | BOLD VALID_ANSWER_TEXT
 *                                   | UNDERLINED VALID_ANSWER_TEXT
 *                                   | CODE VALID_ANSWER_TEXT
 *                                   | TABLE VALID_ANSWER_TEXT
 *
 * VALID_ANSWER_TEXT               ::= TEXT VALID_ANSWER_TEXT
 *                                   | LATEX VALID_ANSWER_TEXT
 *                                   | UNORDERED_LIST VALID_ANSWER_TEXT
 *                                   | ORDERED_LIST VALID_ANSWER_TEXT
 *                                   | BOLD VALID_ANSWER_TEXT
 *                                   | UNDERLINED VALID_ANSWER_TEXT
 *                                   | CODE VALID_ANSWER_TEXT
 *                                   | TABLE VALID_ANSWER_TEXT
 *                                   | EMPTY_WORD
 *
 * TEXT                            ::= TOKEN_WORD OPT_WORD
 *
 * OPT_WORD                        ::= TOKEN_WORD OPT_WORD
 *                                   | EMPTY_WORD
 *
 * LATEX                           ::= NORMAL_LATEX
 *                                   | CENTERED_LATEX
 *
 * NORMAL_LATEX                    ::= TOKEN_LATEX_OPENING LATEX_BODY TOKEN_LATEX_CLOSING
 *
 * CENTERED_LATEX                  ::= TOKEN_LATEX_CENTERED_OPENING LATEX_BODY TOKEN_LATEX_CENTERED_CLOSING
 *
 * LATEX_BODY                      ::= TOKEN_LATEX_CODE OPT_LATEX_CODE
 *
 * OPT_LATEX_CODE                  ::= TOKEN_LATEX_CODE OPT_LATEX_CODE
 *                                   | EMPTY_WORD
 *
 * UNORDERED_LIST                  ::= UNORDERED_LIST_ITEM OPT_UNORDERED_LIST_ITEM
 *
 * UNORDERED_LIST_ITEM             ::= TOKEN_UNORDERED_LIST_ITEM LIST_ITEM_TEXT
 *
 * OPT_UNORDERED_LIST_ITEM         ::= UNORDERED_LIST_ITEM OPT_UNORDERED_LIST_ITEM
 *                                   | EMPTY_WORD
 *
 * ORDERED_LIST                    ::= ORDERED_LIST_ITEM OPT_ORDERED_LIST_ITEM
 *
 * ORDERED_LIST_ITEM               ::= TOKEN_ORDERED_LIST_ITEM LIST_ITEM_TEXT
 *
 * OPT_ORDERED_LIST_ITEM           ::= ORDERED_LIST_ITEM OPT_ORDERED_LIST_ITEM
 *                                   | EMPTY_WORD
 *
 * LIST_ITEM_TEXT                  ::= TEXT VALID_LIST_ITEM_TEXT
 *                                   | LATEX VALID_LIST_ITEM_TEXT
 *                                   | BOLD VALID_LIST_ITEM_TEXT
 *                                   | UNDERLINED VALID_LIST_ITEM_TEXT
 *                                   | CODE VALID_LIST_ITEM_TEXT
 *                                   | TABLE VALID_LIST_ITEM_TEXT
 *
 * VALID_LIST_ITEM_TEXT            ::= TEXT VALID_LIST_ITEM_TEXT
 *                                   | LATEX VALID_LIST_ITEM_TEXT
 *                                   | BOLD VALID_LIST_ITEM_TEXT
 *                                   | UNDERLINED VALID_LIST_ITEM_TEXT
 *                                   | CODE VALID_LIST_ITEM_TEXT
 *                                   | TABLE VALID_LIST_ITEM_TEXT
 *                                   | EMPTY_WORD
 *
 * BOLD                            ::= TOKEN_BOLD_OPENING TEXT TOKEN_BOLD_CLOSING
 *
 * UNDERLINED                      ::= TOKEN_UNDERLINE_OPENING TEXT TOKEN_UNDERLINE_CLOSING
 *
 * CODE                            ::= TOKEN_CODE_OPENING TEXT TOKEN_CODE_CLOSING
 *
 * CHAPTER                         ::= TOKEN_CHAPTER TEXT OPT_QUESTION OPT_SECTION
 * 
 * OPT_CHAPTER                     ::= CHAPTER OPT_CHAPTER
 *                                   | EMPTY_WORD
 *
 * SECTION                         ::= TOKEN_SECTION TEXT OPT_QUESTION OPT_SUBSECTION
 *
 * OPT_SECTION                     ::= SECTION OPT_SECTION
 *                                   | EMPTY_WORD
 *
 * SUBSECTION                      ::= TOKEN_SUBSECTION TEXT OPT_QUESTION
 *
 * OPT_SUBSECTION                  ::= SUBSECTION OPT_SUBSECTION
 *                                   | EMPTY_WORD
 *
 * TABLE                           ::= TOKEN_TABLE_DIVIDER TABLE_ROW OPT_TABLE_ROW
 *
 * TABLE_ROW                       ::= TABLE_CELL OPT_TABLE_CELL TOKEN_TABLE_DIVIDER
 *
 * OPT_TABLE_ROW                   ::= TABLE_ROW OPT_TABLE_ROW
 *                                   | EMPTY_WORD
 *
 * TABLE_CELL                      ::= TOKEN_TABLE_CELL
 *                                   | TOKEN_TABLE_CELL_LEFT_ALIGNED
 *                                   | TOKEN_TABLE_CELL_RIGHT_ALIGNED
 *                                   | TOKEN_TABLE_CELL_CENTER_ALIGNED
 *
 * OPT_TABLE_CELL                  ::= TABLE_CELL OPT_TABLE_CELL
 *                                   | EMPTY_WORD
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
