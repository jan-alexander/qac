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

   private:
    int chapter_counter_ = 0;
    int section_counter_ = 0;
    int subsection_counter_ = 0;
    int question_counter_ = 0;
};
}

#endif  // QAC_GENERATOR_H
