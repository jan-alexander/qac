#ifndef QAC_CST_TO_AST_VISITOR_H
#define QAC_CST_TO_AST_VISITOR_H

#include <qac/parser/ast_nodes.h>
#include <qac/parser/cst_nodes.h>

#include <memory>
#include <sstream>
#include <stack>

#include <boost/algorithm/string.hpp>

namespace qac {

class generator;

enum class cst_to_ast_visitor_state {
    IN_ROOT,
    IN_CHAPTER,
    IN_SECTION,
    IN_SUBSECTION
};

class cst_to_ast_visitor : public cst_visitor {
   public:
    using texts_stack = std::stack<std::ostringstream>;

    cst_to_ast_visitor(generator *generator) : generator_(generator) {}
    ast_node::ptr root() { return std::move(root_); }

    virtual void visit(cst_root_questions *node) override;
    virtual void visit(cst_root_chapters *node) override;
    virtual void visit(cst_question *node) override;
    virtual void visit(cst_question_text *node) override;
    virtual void visit(cst_answer_text *node) override;
    virtual void visit(cst_text *node) override;
    virtual void visit(cst_latex *node) override;
    virtual void visit(cst_normal_latex *node) override;
    virtual void visit(cst_centered_latex *node) override;
    virtual void visit(cst_latex_body *node) override;
    virtual void visit(cst_unordered_list *node) override;
    virtual void visit(cst_unordered_list_item *node) override;
    virtual void visit(cst_ordered_list *node) override;
    virtual void visit(cst_ordered_list_item *node) override;
    virtual void visit(cst_list_item_text *node) override;
    virtual void visit(cst_bold *node) override;
    virtual void visit(cst_underlined *node) override;
    virtual void visit(cst_code *node) override;
    virtual void visit(cst_chapter *node) override;
    virtual void visit(cst_section *node) override;
    virtual void visit(cst_subsection *node) override;
    virtual void visit(cst_table *node) override;
    virtual void visit(cst_table_row *node) override;
    virtual void visit(cst_table_cell *node) override;
    virtual void visit(cst_table_cell_text *node) override;

   private:
    void push_text_stream() { texts_stack_.push(std::ostringstream()); }
    void pop_text_stream() { texts_stack_.pop(); }
    std::ostringstream &text_stream() { return texts_stack_.top(); }

    std::string &trim(std::string &string) {
        boost::algorithm::trim(string);
        return string;
    }

    cst_to_ast_visitor_state state_ = cst_to_ast_visitor_state::IN_ROOT;

    ast_node::ptr root_;

    texts_stack texts_stack_;

    generator *generator_;

    ast_chapter::ptr chapter_;
    ast_section::ptr section_;
    ast_subsection::ptr subsection_;

    uint16_t nth_chapter_ = 0;
    uint16_t nth_section_ = 0;
    uint16_t nth_subsection_ = 0;
    uint16_t nth_question_ = 0;

    const bool LOG_VISIT = true;
    const bool LOG_QUESTION = true;
    const bool LOG_CHAPTER = true;
    const bool LOG_SECTION = true;
    const bool LOG_SUBSECTION = true;
};

}  // namespace qac

#endif  // QAC_CST_TO_AST_VISITOR_H
