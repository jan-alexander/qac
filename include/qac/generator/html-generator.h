#ifndef QAC_HTML_GENERATOR_H_H
#define QAC_HTML_GENERATOR_H_H

#include "qac/generator/generator.h"

namespace qac {

class html_generator : public generator {
public:
    static std::string get_name();

    virtual std::string get_default_file_extension() override;
    virtual std::string get_header() override;
    virtual std::string get_footer() override;
};

}

#endif //QAC_HTML_GENERATOR_H_H
