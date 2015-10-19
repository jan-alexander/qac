#include "qac/parser/parser.h"

#include <set>

/*
 * The grammar
 * ===========
 *
 * ROOT                            ::= QUESTION OPT_QUESTION
 *                                   | CHAPTER OPT_CHAPTER
 *
 * QUESTION                        ::= TOKEN_QUESTION QUESTION_TEXT TOKEN_ANSWER
 *                                         ANSWER_TEXT
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
 * NORMAL_LATEX                    ::= TOKEN_LATEX_OPENING LATEX_BODY
 *                                         TOKEN_LATEX_CLOSING
 *
 * CENTERED_LATEX                  ::= TOKEN_LATEX_CENTERED_OPENING LATEX_BODY
 *                                         TOKEN_LATEX_CENTERED_CLOSING
 *
 * LATEX_BODY                      ::= TOKEN_LATEX_CODE OPT_LATEX_CODE
 *
 * OPT_LATEX_CODE                  ::= TOKEN_LATEX_CODE OPT_LATEX_CODE
 *                                   | EMPTY_WORD
 *
 * UNORDERED_LIST                  ::= UNORDERED_LIST_ITEM
 *                                         OPT_UNORDERED_LIST_ITEM
 *
 * UNORDERED_LIST_ITEM             ::= TOKEN_UNORDERED_LIST_ITEM LIST_ITEM_TEXT
 *
 * OPT_UNORDERED_LIST_ITEM         ::= UNORDERED_LIST_ITEM
 *                                         OPT_UNORDERED_LIST_ITEM
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
 * BOLD                            ::= TOKEN_BOLD_OPENING TEXT
 *                                         TOKEN_BOLD_CLOSING
 *
 * UNDERLINED                      ::= TOKEN_UNDERLINE_OPENING TEXT
 *                                         TOKEN_UNDERLINE_CLOSING
 *
 * CODE                            ::= TOKEN_CODE_OPENING TEXT
 *                                         TOKEN_CODE_CLOSING
 *
 * CHAPTER                         ::= TOKEN_CHAPTER TEXT OPT_QUESTION
 *                                         OPT_SECTION
 *
 * OPT_CHAPTER                     ::= CHAPTER OPT_CHAPTER
 *                                   | EMPTY_WORD
 *
 * SECTION                         ::= TOKEN_SECTION TEXT OPT_QUESTION
 *                                         OPT_SUBSECTION
 *
 * OPT_SECTION                     ::= SECTION OPT_SECTION
 *                                   | EMPTY_WORD
 *
 * SUBSECTION                      ::= TOKEN_SUBSECTION TEXT OPT_QUESTION
 *
 * OPT_SUBSECTION                  ::= SUBSECTION OPT_SUBSECTION
 *                                   | EMPTY_WORD
 *
 * TABLE                           ::= TOKEN_TABLE_DIVIDER TABLE_ROW
 *                                         OPT_TABLE_ROW
 *
 * TABLE_ROW                       ::= TABLE_CELL OPT_TABLE_CELL
 *                                         TOKEN_TABLE_DIVIDER
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
    return parse_root();
}

bool parser::match(token_enum token) { return false; }
token_enum parser::lookahead() { return token_enum::BOLD_CLOSING; }
void parser::no_rule_found() { throw runtime_error("No rule found!"); }

std::unique_ptr<node> parser::parse_root() {
    unique_ptr<node> ret = make_unique<node>(node_enum::ROOT);

    switch (lookahead()) {
        case token_enum::QUESTION:
            ret->add_child(parse_question());
            ret->add_child(parse_question(true));
            break;

        case token_enum::CHAPTER:
            ret->add_child(parse_chapter());
            ret->add_child(parse_chapter(true));
            break;

        default:
            no_rule_found();
            break;
    }

    return ret;
}

std::unique_ptr<node> parser::parse_question(bool optional) {
    unique_ptr<node> ret = make_unique<node>(node_enum::QUESTION);

    if (!optional || lookahead() == token_enum::QUESTION) {
        match(token_enum::QUESTION);
        ret->add_child(parse_question_text());
        match(token_enum::ANSWER);
        ret->add_child(parse_answer_text());
    }

    return ret;
}

std::unique_ptr<node> parser::parse_question_text(bool optional) {
    unique_ptr<node> ret = make_unique<node>(node_enum::QUESTION_TEXT);

    switch (lookahead()) {
        case token_enum::WORD:
            ret->add_child(parse_text());
            ret->add_child(parse_question_text(true));
            break;

        case token_enum::LATEX_OPENING:
        case token_enum::LATEX_CENTERED_OPENING:
            ret->add_child(parse_latex());
            ret->add_child(parse_question_text(true));
            break;

        case token_enum::BOLD_OPENING:
            ret->add_child(parse_bold());
            ret->add_child(parse_question_text(true));
            break;

        case token_enum::UNDERLINE_OPENING:
            ret->add_child(parse_underlined());
            ret->add_child(parse_question_text(true));
            break;

        case token_enum::CODE_OPENING:
            ret->add_child(parse_code());
            ret->add_child(parse_question_text(true));
            break;

        default:
            if (!optional) {
                no_rule_found();
            }
            break;
    }

    return ret;
}

std::unique_ptr<node> parser::parse_answer_text(bool optional) {
    unique_ptr<node> ret = make_unique<node>(node_enum::ANSWER_TEXT);

    switch (lookahead()) {
        case token_enum::WORD:
            ret->add_child(parse_text());
            ret->add_child(parse_answer_text(true));
            break;

        case token_enum::LATEX_OPENING:
        case token_enum::LATEX_CENTERED_OPENING:
            ret->add_child(parse_latex());
            ret->add_child(parse_answer_text(true));
            break;

        case token_enum::UNORDERED_LIST_ITEM:
            ret->add_child(parse_unordered_list());
            ret->add_child(parse_answer_text(true));
            break;

        case token_enum::ORDERED_LIST_ITEM:
            ret->add_child(parse_ordered_list());
            ret->add_child(parse_answer_text(true));
            break;

        case token_enum::BOLD_OPENING:
            ret->add_child(parse_bold());
            ret->add_child(parse_answer_text(true));
            break;

        case token_enum::UNDERLINE_OPENING:
            ret->add_child(parse_underlined());
            ret->add_child(parse_answer_text(true));
            break;

        case token_enum::CODE_OPENING:
            ret->add_child(parse_code());
            ret->add_child(parse_answer_text(true));
            break;

        case token_enum::TABLE_DIVIDER:
            ret->add_child(parse_table());
            ret->add_child(parse_answer_text(true));
            break;

        default:
            if (!optional) {
                no_rule_found();
            }
            break;
    }

    return ret;
}

std::unique_ptr<node> parser::parse_text() {
    unique_ptr<node> ret = make_unique<node>(node_enum::TEXT);

    do {
        match(token_enum::WORD);
    } while (lookahead() == token_enum::WORD);

    return ret;
}

std::unique_ptr<node> parser::parse_latex() {
    unique_ptr<node> ret = make_unique<node>(node_enum::LATEX);

    switch (lookahead()) {
        case token_enum::LATEX_OPENING:
            ret->add_child(parse_normal_latex());
            break;

        case token_enum::LATEX_CENTERED_OPENING:
            ret->add_child(parse_centered_latex());
            break;

        default:
            no_rule_found();
            break;
    }

    return ret;
}

std::unique_ptr<node> parser::parse_normal_latex() {
    unique_ptr<node> ret = make_unique<node>(node_enum::NORMAL_LATEX);

    match(token_enum::LATEX_OPENING);
    ret->add_child(parse_latex_body());
    match(token_enum::LATEX_CLOSING);

    return ret;
}

std::unique_ptr<node> parser::parse_centered_latex() {
    unique_ptr<node> ret = make_unique<node>(node_enum::CENTERED_LATEX);

    match(token_enum::LATEX_CENTERED_OPENING);
    ret->add_child(parse_latex_body());
    match(token_enum::LATEX_CENTERED_CLOSING);

    return ret;
}

std::unique_ptr<node> parser::parse_latex_body() {
    unique_ptr<node> ret = make_unique<node>(node_enum::LATEX_BODY);

    do {
        match(token_enum::LATEX_CODE);
    } while (lookahead() == token_enum::LATEX_CODE);

    return ret;
}

std::unique_ptr<node> parser::parse_unordered_list() {
    unique_ptr<node> ret = make_unique<node>(node_enum::UNORDERED_LIST);

    if (lookahead() == token_enum::UNORDERED_LIST_ITEM) {
        ret->add_child(parse_unordered_list_item());
        ret->add_child(parse_unordered_list_item(true));
    } else {
        no_rule_found();
    }

    return ret;
}

std::unique_ptr<node> parser::parse_unordered_list_item(bool optional) {
    unique_ptr<node> ret = make_unique<node>(node_enum::UNORDERED_LIST_ITEM);

    if (!optional || lookahead() == token_enum::UNORDERED_LIST_ITEM) {
        match(token_enum::UNORDERED_LIST_ITEM);
        ret->add_child(parse_list_item_text());
    }

    return ret;
}

std::unique_ptr<node> parser::parse_ordered_list() {
    unique_ptr<node> ret = make_unique<node>(node_enum::ORDERED_LIST);

    if (lookahead() == token_enum::ORDERED_LIST_ITEM) {
        ret->add_child(parse_ordered_list_item());
        ret->add_child(parse_ordered_list_item(true));
    } else {
        no_rule_found();
    }

    return ret;
}

std::unique_ptr<node> parser::parse_ordered_list_item(bool optional) {
    unique_ptr<node> ret = make_unique<node>(node_enum::ORDERED_LIST_ITEM);

    if (!optional || lookahead() == token_enum::ORDERED_LIST_ITEM) {
        match(token_enum::ORDERED_LIST_ITEM);
        ret->add_child(parse_list_item_text());
    }

    return ret;
}

std::unique_ptr<node> parser::parse_list_item_text(bool optional) {
    unique_ptr<node> ret = make_unique<node>(node_enum::LIST_ITEM_TEXT);

    switch (lookahead()) {
        case token_enum::WORD:
            ret->add_child(parse_text());
            ret->add_child(parse_list_item_text(true));
            break;

        case token_enum::LATEX_OPENING:
        case token_enum::LATEX_CENTERED_OPENING:
            ret->add_child(parse_latex());
            ret->add_child(parse_list_item_text(true));
            break;

        case token_enum::BOLD_OPENING:
            ret->add_child(parse_bold());
            ret->add_child(parse_list_item_text(true));
            break;

        case token_enum::UNDERLINE_OPENING:
            ret->add_child(parse_underlined());
            ret->add_child(parse_list_item_text(true));
            break;

        case token_enum::CODE_OPENING:
            ret->add_child(parse_code());
            ret->add_child(parse_list_item_text(true));
            break;

        case token_enum::TABLE_DIVIDER:
            ret->add_child(parse_table());
            ret->add_child(parse_list_item_text(true));
            break;

        default:
            if (!optional) {
                no_rule_found();
            }
            break;
    }

    return ret;
}

std::unique_ptr<node> parser::parse_bold() {
    unique_ptr<node> ret = make_unique<node>(node_enum::BOLD);

    match(token_enum::BOLD_OPENING);
    ret->add_child(parse_text());
    match(token_enum::BOLD_CLOSING);

    return ret;
}

std::unique_ptr<node> parser::parse_underlined() {
    unique_ptr<node> ret = make_unique<node>(node_enum::UNDERLINED);

    match(token_enum::UNDERLINE_OPENING);
    ret->add_child(parse_text());
    match(token_enum::UNDERLINE_CLOSING);

    return ret;
}

std::unique_ptr<node> parser::parse_code() {
    unique_ptr<node> ret = make_unique<node>(node_enum::CODE);

    match(token_enum::CODE_OPENING);
    ret->add_child(parse_text());
    match(token_enum::CODE_CLOSING);

    return ret;
}

std::unique_ptr<node> parser::parse_chapter(bool optional) {
    unique_ptr<node> ret = make_unique<node>(node_enum::CHAPTER);

    if (!optional || lookahead() == token_enum::CHAPTER) {
        match(token_enum::CHAPTER);
        ret->add_child(parse_text());
        ret->add_child(parse_question(true));
        ret->add_child(parse_section(true));
    }

    return ret;
}

std::unique_ptr<node> parser::parse_section(bool optional) {
    unique_ptr<node> ret = make_unique<node>(node_enum::SECTION);

    if (!optional || lookahead() == token_enum::SECTION) {
        match(token_enum::SECTION);
        ret->add_child(parse_text());
        ret->add_child(parse_question(true));
        ret->add_child(parse_subsection(true));
    }

    return ret;
}

std::unique_ptr<node> parser::parse_subsection(bool optional) {
    unique_ptr<node> ret = make_unique<node>(node_enum::SUBSECTION);

    if (!optional || lookahead() == token_enum::SUBSECTION) {
        match(token_enum::SUBSECTION);
        ret->add_child(parse_text());
        ret->add_child(parse_question(true));
        ret->add_child(parse_subsection(true));
    }

    return ret;
}

std::unique_ptr<node> parser::parse_table() {
    unique_ptr<node> ret = make_unique<node>(node_enum::TABLE);

    match(token_enum::TABLE_DIVIDER);
    ret->add_child(parse_table_row());
    ret->add_child(parse_table_row(true));

    return ret;
}

std::unique_ptr<node> parser::parse_table_row(bool optional) {
    unique_ptr<node> ret = make_unique<node>(node_enum::TABLE_ROW);

    switch(lookahead()) {
        case token_enum::TABLE_CELL:
        case token_enum::TABLE_CELL_LEFT_ALIGNED:
        case token_enum::TABLE_CELL_RIGHT_ALIGNED:
        case token_enum::TABLE_CELL_CENTER_ALIGNED:
            ret->add_child(parse_table_cell());
            ret->add_child(parse_table_cell(true));
            match(token_enum::TABLE_DIVIDER);
            break;

        default:
            if(!optional) {
                no_rule_found();
            }
            break;
    }

    return ret;
}

std::unique_ptr<node> parser::parse_table_cell(bool optional) {
    unique_ptr<node> ret = make_unique<node>(node_enum::TABLE_CELL);

    switch (lookahead()) {
        case token_enum::TABLE_CELL:
            match(token_enum::TABLE_CELL);
            break;

        case token_enum::TABLE_CELL_LEFT_ALIGNED:
            match(token_enum::TABLE_CELL_LEFT_ALIGNED);
            break;

        case token_enum::TABLE_CELL_RIGHT_ALIGNED:
            match(token_enum::TABLE_CELL_RIGHT_ALIGNED);
            break;

        case token_enum::TABLE_CELL_CENTER_ALIGNED:
            match(token_enum::TABLE_CELL_CENTER_ALIGNED);
            break;

        default:
            if (!optional) {
                no_rule_found();
            }
            break;
    }

    return ret;
}
