#ifndef QAC_HTML_GENERATOR_H_H
#define QAC_HTML_GENERATOR_H_H

#include "qac/generator/generator.h"

namespace qac {

class html_generator : public generator {
   public:
    virtual std::string get_name() override;
    virtual std::string get_description() override;

   protected:
    virtual void render_bold(std::ostream &os,
                             const std::string &text) override;
    virtual void render_underlined(std::ostream &os,
                                   const std::string &text) override;
    virtual void render_code(std::ostream &os,
                             const std::string &text) override;
    virtual void render_unordered_list(std::ostream &os,
                                       const std::string &text) override;
    virtual void render_unordered_list_item(std::ostream &os,
                                            const std::string &text) override;
    virtual void render_ordered_list(std::ostream &os,
                                     const std::string &text) override;
    virtual void render_ordered_list_item(std::ostream &os,
                                          const std::string &text) override;
    virtual void render_chapter(std::ostream &os, const std::string &caption,
                                const std::string &questions,
                                const std::string &sections,
                                const ast_chapter *chapter) override;

    virtual void render_section(std::ostream &os, const std::string &caption,
                                const std::string &questions,
                                const std::string &subsections,
                                const ast_section *section) override;

    virtual void render_subsection(std::ostream &os, const std::string &caption,
                                   const std::string &questions,
                                   const ast_subsection *subsection) override;

    virtual void render_question(std::ostream &os, const std::string &question,
                                 const std::string &answer,
                                 const ast_question *pquestion) override;

    virtual void render_document(std::ostream &os,
                                 const std::string &body) override;
};
}

#endif  // QAC_HTML_GENERATOR_H_H
