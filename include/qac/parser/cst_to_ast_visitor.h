#ifndef QAC_CST_TO_AST_VISITOR_H
#define QAC_CST_TO_AST_VISITOR_H

#include <qac/parser/ast_nodes.h>
#include <qac/parser/cst_nodes.h>

#include <memory>
#include <sstream>
#include <stack>

namespace qac {

class generator;

class cst_to_ast_visitor : public cst_visitor {
   public:
    cst_to_ast_visitor(generator *generator) : generator_(generator) {}
    std::unique_ptr<ast_node> get_root();

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

   private:
    std::string text();

    void push_text_stream() {
        texts_stack_.push(std::ostringstream());
    }

    void pop_text_stream() {
        texts_stack_.pop();
    }

    std::ostringstream &text_stream() {
        return texts_stack_.top();
    }

    std::stack<std::ostringstream> texts_stack_;
    generator *generator_;
};

}  // namespace qac

#endif  // QAC_CST_TO_AST_VISITOR_H
