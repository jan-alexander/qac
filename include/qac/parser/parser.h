#ifndef QAC_PARSER_H
#define QAC_PARSER_H

#include <memory>
#include <vector>
#include <qac/lexer/lexer.h>
#include <qac/parser/cst_nodes.h>

namespace qac {

class parser {
   public:
    std::unique_ptr<cst_node> parse(const std::vector<token> &tokens);

   private:
    bool match(token_enum token);
    token_enum lookahead();
    void skip_whitespace();
    void no_rule_found(cst_node_enum nenum);

    std::unique_ptr<cst_node> parse_root();
    std::unique_ptr<cst_node> parse_question(bool optional = false);
    std::unique_ptr<cst_node> parse_question_text(bool optional = false);
    std::unique_ptr<cst_node> parse_answer_text(bool optional = false);
    std::unique_ptr<cst_node> parse_text();
    std::unique_ptr<cst_node> parse_latex();
    std::unique_ptr<cst_node> parse_normal_latex();
    std::unique_ptr<cst_node> parse_centered_latex();
    std::unique_ptr<cst_node> parse_latex_body();
    std::unique_ptr<cst_node> parse_unordered_list();
    std::unique_ptr<cst_node> parse_unordered_list_item(bool optional = false);
    std::unique_ptr<cst_node> parse_ordered_list();
    std::unique_ptr<cst_node> parse_ordered_list_item(bool optional = false);
    std::unique_ptr<cst_node> parse_list_item_text(bool optional = false);
    std::unique_ptr<cst_node> parse_bold();
    std::unique_ptr<cst_node> parse_underlined();
    std::unique_ptr<cst_node> parse_code();
    std::unique_ptr<cst_node> parse_chapter(bool optional = false);
    std::unique_ptr<cst_node> parse_section(bool optional = false);
    std::unique_ptr<cst_node> parse_subsection(bool optional = false);
    std::unique_ptr<cst_node> parse_table();
    std::unique_ptr<cst_node> parse_table_row(bool optional = false);
    std::unique_ptr<cst_node> parse_table_cell(bool optional = false);

    std::vector<token>::const_iterator current_;
    std::vector<token>::const_iterator lookahead_;
    std::vector<token>::const_iterator end_;

    uint16_t cur_line_ = 1;
};
}

#endif  // QAC_PARSER_H
