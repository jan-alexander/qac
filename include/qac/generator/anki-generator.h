#ifndef QAC_ANKI_GENERATOR_H
#define QAC_ANKI_GENERATOR_H

#include <qac/generator/html-generator.h>

namespace qac {

class anki_generator : public html_generator {
   public:
    virtual std::string get_name() override;
    virtual std::string get_description() override;

   protected:
    virtual void render_image(std::ostream &os, const std::string &source,
                              int width, int height) override;

    virtual void render_underlined(std::ostream &os,
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

    virtual void render_table_cell(std::ostream &os,
                                   const std::string &cell_body) override;
    virtual void render_table_cell_left_aligned(
        std::ostream &os, const std::string &cell_body) override;
    virtual void render_table_cell_right_aligned(
        std::ostream &os, const std::string &cell_body) override;
    virtual void render_table_cell_center_aligned(
        std::ostream &os, const std::string &cell_body) override;
    virtual void render_table(std::ostream &os,
                              const std::string &rows) override;

    virtual void render_normal_latex(std::ostream &os, const std::string &text) override;
    virtual void render_centered_latex(std::ostream &os, const std::string &text) override;
};
}  // namespace qac

#endif
