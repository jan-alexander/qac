#ifndef QAC_HTML_GENERATOR_H_H
#define QAC_HTML_GENERATOR_H_H

#include "qac/generator/generator.h"

namespace qac {

class html_generator : public generator {
   public:
    virtual std::string get_name() override;
    virtual std::string get_description() override;
};
}

#endif  // QAC_HTML_GENERATOR_H_H
