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
    virtual void render_unordered_list_item(std::ostream &os,
                                            const std::string &text) override;
    virtual void render_ordered_list_item(std::ostream &os,
                                          const std::string &text) override;
};
}

#endif  // QAC_HTML_GENERATOR_H_H
