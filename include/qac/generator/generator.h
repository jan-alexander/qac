#ifndef QAC_GENERATOR_H
#define QAC_GENERATOR_H

#include <qac/parser/parser.h>
#include <qac/parser/ast_nodes.h>

#include <string>

namespace qac {

class generator {
   public:
    virtual std::string get_name() = 0;
    virtual std::string get_description() = 0;

    void generate(qac::cst_node *root, std::ostream &os);

    virtual void render_bold(std::ostream &os, const std::string &text) = 0;
    virtual void render_underlined(std::ostream &os,
                                   const std::string &text) = 0;
    virtual void render_code(std::ostream &os, const std::string &text) = 0;
    virtual void render_unordered_list(std::ostream &os,
                                       const std::string &text) = 0;
    virtual void render_unordered_list_item(std::ostream &os,
                                            const std::string &text) = 0;
    virtual void render_ordered_list(std::ostream &os,
                                     const std::string &text) = 0;
    virtual void render_ordered_list_item(std::ostream &os,
                                          const std::string &text) = 0;

    void render_normal_latex(std::ostream &os, const std::string &text);
    void render_centered_latex(std::ostream &os, const std::string &text);

    virtual void render_chapter(std::ostream &os, const std::string &caption,
                                const std::string &questions,
                                const std::string &sections,
                                const ast_chapter *chapter) = 0;

    virtual void render_section(std::ostream &os, const std::string &caption,
                                const std::string &questions,
                                const std::string &subsections,
                                const ast_section *section) = 0;

    virtual void render_subsection(std::ostream &os, const std::string &caption,
                                   const std::string &questions,
                                   const ast_subsection *subsection) = 0;

    virtual void render_question(std::ostream &os, const std::string &question,
                                 const std::string &answer,
                                 const ast_question *pquestion) = 0;

    virtual void render_document(std::ostream &os, const std::string &body) = 0;

   private:
    int chapter_counter_ = 0;
    int section_counter_ = 0;
    int subsection_counter_ = 0;
    int question_counter_ = 0;
};
}

#endif  // QAC_GENERATOR_H
