#ifndef QAC_GENERATOR_H
#define QAC_GENERATOR_H

#include <string>

#include "qac/parser/parser.h"

namespace qac {

class generator {
public:
    static std::string get_name();

    virtual std::string get_default_file_extension() = 0;
    virtual std::string get_header() = 0;
    virtual std::string get_footer() = 0;

    void generate(qac::node *root, const char* input_file, const char* output_file);

private:
    int chapter_counter_ = 0;
    int section_counter_ = 0;
    int subsection_counter_ = 0;
    int question_counter_ = 0;
};

}

#endif //QAC_GENERATOR_H
