#ifndef QAC_GENERATOR_H
#define QAC_GENERATOR_H

#include <string>

#include "qac/parser/parser.h"

namespace qac {

class generator {
   public:
    virtual std::string get_name() = 0;
    virtual std::string get_description() = 0;

    void generate(qac::node *root, std::ostream &os);

   protected:
    virtual void render_bold(std::ostream &os, const std::string &text) = 0;
    virtual void render_underlined(std::ostream &os, const std::string &text) = 0;
    virtual void render_code(std::ostream &os, const std::string &text) = 0;
    virtual void render_unordered_list_item(std::ostream &os, const std::string &text) = 0;
    virtual void render_ordered_list_item(std::ostream &os, const std::string &text) = 0;

   private:
    void render_normal_latex(std::ostream &os, const std::string &text);
    void render_centered_latex(std::ostream &os, const std::string &text);

    int chapter_counter_ = 0;
    int section_counter_ = 0;
    int subsection_counter_ = 0;
    int question_counter_ = 0;
};
}

#endif  // QAC_GENERATOR_H
