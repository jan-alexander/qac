#ifndef QAC_AST_RENDER_VISITOR_H
#define QAC_AST_RENDER_VISITOR_H

#include <qac/parser/ast_nodes.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <stack>

namespace qac {

class generator;

class ast_render_visitor : public ast_visitor {
   public:
    using texts_stack = std::stack<std::unique_ptr<std::ostringstream>>;

    ast_render_visitor(generator *generator) : generator_(generator) {}

    const std::string &rendered_qa() const { return rendered_qa_; }

    virtual void visit(ast_chapter *node) override;
    virtual void visit(ast_question *node) override;
    virtual void visit(ast_root_chapters *node) override;
    virtual void visit(ast_root_questions *node) override;
    virtual void visit(ast_section *node) override;
    virtual void visit(ast_subsection *node) override;

   private:
    void push_text_stream() {
        texts_stack_.push(std::make_unique<std::ostringstream>());
    }
    void pop_text_stream() { texts_stack_.pop(); }
    std::ostringstream &text_stream() { return *texts_stack_.top(); }

    generator *generator_;
    texts_stack texts_stack_;
    std::string rendered_qa_;

    const bool LOG_VISIT = true;
    const bool LOG_QUESTION = true;
    const bool LOG_CHAPTER = true;
    const bool LOG_SECTION = true;
    const bool LOG_SUBSECTION = true;
};

}  // namespace qac

#endif
