#ifndef QAC_PARSER_H
#define QAC_PARSER_H

#include <memory>
#include <vector>
#include <qac/lexer/lexer.h>

namespace qac {

enum class node_enum {
    ROOT,
    QUESTION,
    QUESTION_TEXT,
    ANSWER_TEXT,
    TEXT,
    LATEX,
    NORMAL_LATEX,
    CENTERED_LATEX,
    LATEX_BODY,
    UNORDERED_LIST,
    UNORDERED_LIST_ITEM,
    ORDERED_LIST,
    ORDERED_LIST_ITEM,
    LIST_ITEM_TEXT,
    BOLD,
    UNDERLINED,
    CODE,
    CHAPTER,
    SECTION,
    SUBSECTION,
    TABLE,
    TABLE_ROW,
    TABLE_CELL,
};

std::string to_string(const node_enum &nenum);
std::ostream &operator<<(std::ostream &os, const node_enum &nenum);

class node {
   public:
    node(node_enum type) : node(type, "") {}

    node(node_enum type, std::string value) : type_(type), value_(value) {}

    void add_child(std::unique_ptr<node> child) {
        children_.push_back(std::move(child));
    }

    node_enum type() const { return type_; }

    const std::vector<std::unique_ptr<node>> &children() const {
        return children_;
    }

    const std::string &value() { return value_; }

   private:
    node_enum type_;
    std::vector<std::unique_ptr<node>> children_;
    std::string value_;
};

class parser {
   public:
    std::unique_ptr<node> parse(const std::vector<token> &tokens);

   private:
    bool match(token_enum token);
    token_enum lookahead();
    void skip_whitespace();
    void no_rule_found(node_enum nenum);

    std::unique_ptr<node> parse_root();
    std::unique_ptr<node> parse_question(bool optional = false);
    std::unique_ptr<node> parse_question_text(bool optional = false);
    std::unique_ptr<node> parse_answer_text(bool optional = false);
    std::unique_ptr<node> parse_text();
    std::unique_ptr<node> parse_latex();
    std::unique_ptr<node> parse_normal_latex();
    std::unique_ptr<node> parse_centered_latex();
    std::unique_ptr<node> parse_latex_body();
    std::unique_ptr<node> parse_unordered_list();
    std::unique_ptr<node> parse_unordered_list_item(bool optional = false);
    std::unique_ptr<node> parse_ordered_list();
    std::unique_ptr<node> parse_ordered_list_item(bool optional = false);
    std::unique_ptr<node> parse_list_item_text(bool optional = false);
    std::unique_ptr<node> parse_bold();
    std::unique_ptr<node> parse_underlined();
    std::unique_ptr<node> parse_code();
    std::unique_ptr<node> parse_chapter(bool optional = false);
    std::unique_ptr<node> parse_section(bool optional = false);
    std::unique_ptr<node> parse_subsection(bool optional = false);
    std::unique_ptr<node> parse_table();
    std::unique_ptr<node> parse_table_row(bool optional = false);
    std::unique_ptr<node> parse_table_cell(bool optional = false);

    std::vector<token>::const_iterator current_;
    std::vector<token>::const_iterator lookahead_;
    std::vector<token>::const_iterator end_;

    uint16_t cur_line_ = 1;
};
}

#endif  // QAC_PARSER_H
