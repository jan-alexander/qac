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
    using chapter_stack = std::stack<ast_chapter::ptr>;
    using section_stack = std::stack<ast_section::ptr>;
    using subsection_stack = std::stack<ast_subsection::ptr>;

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
    void push_text_stream() { texts_stack_.push(std::ostringstream()); }
    void pop_text_stream() { texts_stack_.pop(); }
    std::ostringstream &text_stream() { return texts_stack_.top(); }

    std::string &trim(std::string &string) {
        boost::algorithm::trim(string);
        return string;
    }

    cst_to_ast_visitor_state state_ = cst_to_ast_visitor_state::IN_ROOT;

    std::stack<std::ostringstream> texts_stack_;
    chapter_stack chapter_stack_;
    section_stack section_stack_;
    subsection_stack subsection_stack_;

    generator *generator_;

    uint16_t nth_chapter_ = 0;
    uint16_t nth_question_ = 0;

    const bool LOG_VISIT = true;
    const bool LOG_QUESTION = true;
    const bool LOG_CHAPTER = true;
    const bool LOG_SECTION = true;
    const bool LOG_SUBSECTION = true;
};

}  // namespace qac

#endif  // QAC_CST_TO_AST_VISITOR_H
