#include "qac/parser/parser.h"

#include <set>
#include <iostream>
#include <sstream>

#include <glog/logging.h>

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

unique_ptr<cst_node> parser::parse(const std::vector<token> &tokens) {
    DLOG(INFO) << "Start parsing";

    current_ = lookahead_ = cbegin(tokens);
    end_ = cend(tokens);

    while (current_ != end_) {
        DLOG(INFO) << *current_++;
    }

    return parse_root();
}

bool parser::match(token_enum token) {
    if (lookahead_->get_token() == token) {
        DLOG(INFO) << "matched " << token;
        current_ = lookahead_;
        cur_line_ = current_->line();
        lookahead_++;
        return true;
    } else {
        DLOG(ERROR) << "can't match " << token;

        std::ostringstream oss;

        oss << "Line " << cur_line_ << ": Match failed. Expected " << token
            << ", but got " << lookahead_->get_token() << ".";

        throw runtime_error(oss.str());
    }

    return false;
}

token_enum parser::lookahead() {
    skip_whitespace();
    return lookahead_ != end_ ? lookahead_->get_token()
                              : token_enum::END_OF_FILE;
}

void parser::skip_whitespace() {
    while (lookahead_ != end_ &&
           (lookahead_->get_token() == token_enum::NEW_LINE ||
            lookahead_->get_token() == token_enum::EMPTY_LINE)) {
        lookahead_++;
        cur_line_++;
    }
}

void parser::no_rule_found(cst_node_enum nenum) {
    std::ostringstream oss;

    oss << "Line " << cur_line_ << ": Trying to parse " << nenum
        << ", but found no applicable rule.";

     throw runtime_error(oss.str());
}

std::unique_ptr<cst_node> parser::parse_root() {
    DLOG(INFO) << "parse_root";
    unique_ptr<cst_node> ret = nullptr;

    switch (lookahead()) {
        case token_enum::QUESTION:
            ret = make_unique<cst_root_questions>();
            ret->add_child(parse_question());
            ret->add_child(parse_question(true));
            break;

        case token_enum::CHAPTER:
            ret = make_unique<cst_root_chapters>();
            ret->add_child(parse_chapter());
            ret->add_child(parse_chapter(true));
            break;

        default:
            no_rule_found(cst_node_enum::ROOT);
            break;
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_question(bool optional) {
    DLOG(INFO) << "parse_question optional? " << optional;
    unique_ptr<cst_node> ret = make_unique<cst_question>();

    if (!optional || lookahead() == token_enum::QUESTION) {
        match(token_enum::QUESTION);
        ret->add_child(parse_question_text());
        match(token_enum::ANSWER);
        ret->add_child(parse_answer_text());
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_question_text(bool optional) {
    DLOG(INFO) << "parse_question_text optional? " << optional;
    unique_ptr<cst_node> ret = make_unique<cst_question_text>();

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
                no_rule_found(cst_node_enum::QUESTION_TEXT);
                return nullptr;
            }
            break;
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_answer_text(bool optional) {
    DLOG(INFO) << "parse_answer_text optional? " << optional;
    unique_ptr<cst_node> ret = make_unique<cst_answer_text>();

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
                no_rule_found(cst_node_enum::ANSWER_TEXT);
                return nullptr;
            }
            break;
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_text() {
    DLOG(INFO) << "parse_text";
    unique_ptr<cst_node> ret = make_unique<cst_text>();

    do {
        match(token_enum::WORD);
    } while (lookahead() == token_enum::WORD);

    return ret;
}

std::unique_ptr<cst_node> parser::parse_latex() {
    DLOG(INFO) << "parse_latex";
    unique_ptr<cst_node> ret = make_unique<cst_latex>();

    switch (lookahead()) {
        case token_enum::LATEX_OPENING:
            ret->add_child(parse_normal_latex());
            break;

        case token_enum::LATEX_CENTERED_OPENING:
            ret->add_child(parse_centered_latex());
            break;

        default:
            no_rule_found(cst_node_enum::LATEX);
            return nullptr;
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_normal_latex() {
    DLOG(INFO) << "parse_normal_latex";
    unique_ptr<cst_node> ret = make_unique<cst_normal_latex>();

    match(token_enum::LATEX_OPENING);
    ret->add_child(parse_latex_body());
    match(token_enum::LATEX_CLOSING);

    return ret;
}

std::unique_ptr<cst_node> parser::parse_centered_latex() {
    DLOG(INFO) << "parse_centered_latex";
    unique_ptr<cst_node> ret = make_unique<cst_centered_latex>();

    match(token_enum::LATEX_CENTERED_OPENING);
    ret->add_child(parse_latex_body());
    match(token_enum::LATEX_CENTERED_CLOSING);

    return ret;
}

std::unique_ptr<cst_node> parser::parse_latex_body() {
    DLOG(INFO) << "parse_latex_body";
    unique_ptr<cst_node> ret = make_unique<cst_latex_body>();

    do {
        match(token_enum::LATEX_CODE);
    } while (lookahead() == token_enum::LATEX_CODE);

    return ret;
}

std::unique_ptr<cst_node> parser::parse_unordered_list() {
    DLOG(INFO) << "parse_unordered_list";
    unique_ptr<cst_node> ret = make_unique<cst_unordered_list>();

    if (lookahead() == token_enum::UNORDERED_LIST_ITEM) {
        ret->add_child(parse_unordered_list_item());
        ret->add_child(parse_unordered_list_item(true));
    } else {
        no_rule_found(cst_node_enum::UNORDERED_LIST);
        return nullptr;
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_unordered_list_item(bool optional) {
    DLOG(INFO) << "parse_unordered_list_item optional? " << optional;
    unique_ptr<cst_node> ret = make_unique<cst_unordered_list_item>();

    if (!optional || lookahead() == token_enum::UNORDERED_LIST_ITEM) {
        match(token_enum::UNORDERED_LIST_ITEM);
        ret->add_child(parse_list_item_text());
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_ordered_list() {
    DLOG(INFO) << "parse_ordered_list";
    unique_ptr<cst_node> ret = make_unique<cst_ordered_list>();

    if (lookahead() == token_enum::ORDERED_LIST_ITEM) {
        ret->add_child(parse_ordered_list_item());
        ret->add_child(parse_ordered_list_item(true));
    } else {
        no_rule_found(cst_node_enum::ORDERED_LIST);
        return nullptr;
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_ordered_list_item(bool optional) {
    DLOG(INFO) << "parse_ordered_list_item optional? " << optional;
    unique_ptr<cst_node> ret = make_unique<cst_ordered_list_item>();

    if (!optional || lookahead() == token_enum::ORDERED_LIST_ITEM) {
        match(token_enum::ORDERED_LIST_ITEM);
        ret->add_child(parse_list_item_text());
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_list_item_text(bool optional) {
    DLOG(INFO) << "parse_list_item_text optional? " << optional;
    unique_ptr<cst_node> ret = make_unique<cst_list_item_text>();

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
                no_rule_found(cst_node_enum::LIST_ITEM_TEXT);
                return nullptr;
            }
            break;
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_bold() {
    DLOG(INFO) << "parse_bold";
    unique_ptr<cst_node> ret = make_unique<cst_bold>();

    match(token_enum::BOLD_OPENING);
    ret->add_child(parse_text());
    match(token_enum::BOLD_CLOSING);

    return ret;
}

std::unique_ptr<cst_node> parser::parse_underlined() {
    DLOG(INFO) << "parse_underlined";
    unique_ptr<cst_node> ret = make_unique<cst_underlined>();

    match(token_enum::UNDERLINE_OPENING);
    ret->add_child(parse_text());
    match(token_enum::UNDERLINE_CLOSING);

    return ret;
}

std::unique_ptr<cst_node> parser::parse_code() {
    DLOG(INFO) << "parse_code";
    unique_ptr<cst_node> ret = make_unique<cst_code>();

    match(token_enum::CODE_OPENING);
    ret->add_child(parse_text());
    match(token_enum::CODE_CLOSING);

    return ret;
}

std::unique_ptr<cst_node> parser::parse_chapter(bool optional) {
    DLOG(INFO) << "parse_chapter optional? " << optional;
    unique_ptr<cst_node> ret = make_unique<cst_chapter>();

    if (!optional || lookahead() == token_enum::CHAPTER) {
        match(token_enum::CHAPTER);
        ret->add_child(parse_text());
        ret->add_child(parse_question(true));
        ret->add_child(parse_section(true));
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_section(bool optional) {
    DLOG(INFO) << "parse_section optional? " << optional;
    unique_ptr<cst_node> ret = make_unique<cst_section>();

    if (!optional || lookahead() == token_enum::SECTION) {
        match(token_enum::SECTION);
        ret->add_child(parse_text());
        ret->add_child(parse_question(true));
        ret->add_child(parse_subsection(true));
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_subsection(bool optional) {
    DLOG(INFO) << "parse_subsection optional? " << optional;
    unique_ptr<cst_node> ret = make_unique<cst_subsection>();

    if (!optional || lookahead() == token_enum::SUBSECTION) {
        match(token_enum::SUBSECTION);
        ret->add_child(parse_text());
        ret->add_child(parse_question(true));
        ret->add_child(parse_subsection(true));
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_table() {
    DLOG(INFO) << "parse_table";
    unique_ptr<cst_node> ret = make_unique<cst_table>();

    match(token_enum::TABLE_DIVIDER);
    ret->add_child(parse_table_row());
    ret->add_child(parse_table_row(true));

    return ret;
}

std::unique_ptr<cst_node> parser::parse_table_row(bool optional) {
    DLOG(INFO) << "parse_table_row optional? " << optional;
    unique_ptr<cst_node> ret = make_unique<cst_table_row>();

    switch (lookahead()) {
        case token_enum::TABLE_CELL:
        case token_enum::TABLE_CELL_LEFT_ALIGNED:
        case token_enum::TABLE_CELL_RIGHT_ALIGNED:
        case token_enum::TABLE_CELL_CENTER_ALIGNED:
            ret->add_child(parse_table_cell());
            ret->add_child(parse_table_cell(true));
            match(token_enum::TABLE_DIVIDER);
            break;

        default:
            if (!optional) {
                no_rule_found(cst_node_enum::TABLE_ROW);
                return nullptr;
            }
            break;
    }

    return ret;
}

std::unique_ptr<cst_node> parser::parse_table_cell(bool optional) {
    DLOG(INFO) << "parse_table_cell optional? " << optional;
    unique_ptr<cst_node> ret = make_unique<cst_table_cell>();

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
                no_rule_found(cst_node_enum::TABLE_CELL);
                return nullptr;
            }
            break;
    }

    return ret;
}

std::string qac::to_string(const cst_node_enum &nenum) {
    switch (nenum) {
        case cst_node_enum::ROOT:
            return "ROOT";
        case cst_node_enum::QUESTION:
            return "QUESTION";
        case cst_node_enum::QUESTION_TEXT:
            return "QUESTION_TEXT";
        case cst_node_enum::ANSWER_TEXT:
            return "ANSWER_TEXT";
        case cst_node_enum::TEXT:
            return "TEXT";
        case cst_node_enum::LATEX:
            return "LATEX";
        case cst_node_enum::NORMAL_LATEX:
            return "NORMAL_LATEX";
        case cst_node_enum::CENTERED_LATEX:
            return "CENTERED_LATEX";
        case cst_node_enum::LATEX_BODY:
            return "LATEX_BODY";
        case cst_node_enum::UNORDERED_LIST:
            return "UNORDERED_LIST";
        case cst_node_enum::UNORDERED_LIST_ITEM:
            return "UNORDERED_LIST_ITEM";
        case cst_node_enum::ORDERED_LIST:
            return "ORDERED_LIST";
        case cst_node_enum::ORDERED_LIST_ITEM:
            return "ORDERED_LIST_ITEM";
        case cst_node_enum::LIST_ITEM_TEXT:
            return "LIST_ITEM_TEXT";
        case cst_node_enum::BOLD:
            return "BOLD";
        case cst_node_enum::UNDERLINED:
            return "UNDERLINED";
        case cst_node_enum::CODE:
            return "CODE";
        case cst_node_enum::CHAPTER:
            return "CHAPTER";
        case cst_node_enum::SECTION:
            return "SECTION";
        case cst_node_enum::SUBSECTION:
            return "SUBSECTION";
        case cst_node_enum::TABLE:
            return "TABLE";
        case cst_node_enum::TABLE_ROW:
            return "TABLE_ROW";
        case cst_node_enum::TABLE_CELL:
            return "TABLE_CELL";
        default:
            return ">>UNKNOWN_NODE<<";
    }
}

std::ostream &qac::operator<<(std::ostream &os, const cst_node_enum &nenum) {
    os << to_string(nenum);
    return os;
}
