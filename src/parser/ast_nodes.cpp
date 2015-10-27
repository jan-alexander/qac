#include <qac/parser/ast_nodes.h>

using namespace qac;
using namespace std;

uint16_t ast_question::nth_chapter() const { return chapter_->nth_chapter(); }
uint16_t ast_question::nth_section() const { return section_->nth_section(); }
uint16_t ast_question::nth_subsection() const {
    return subsection_->nth_subsection();
}
const std::string &ast_question::chapter() const { return chapter_->chapter(); }
const std::string &ast_question::section() const { return section_->section(); }
const std::string &ast_question::subsection() const {
    return subsection_->subsection();
}
