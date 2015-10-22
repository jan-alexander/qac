#ifndef QAC_AST_NODES_H
#define QAC_AST_NODES_H

#include <memory>
#include <string>
#include <vector>

namespace qac {

class ast_chapter;
class ast_section;
class ast_subsection;

class ast_node {};

class ast_question : public ast_node {
   private:
    std::string question_;
    std::string answer_;

    uint16_t nth_question_ = 0;

    ast_chapter *chapter_ = nullptr;
    ast_section *section_ = nullptr;
    ast_subsection *subsection_ = nullptr;
};

class ast_subsection : public ast_node {
   private:
    std::string subsection_;

    uint16_t nth_subsection_ = 0;

    ast_chapter *chapter_ = nullptr;
    ast_section *section_ = nullptr;

    std::vector<std::unique_ptr<ast_question>> questions_;
};

class ast_section : public ast_node {
   private:
    std::string section_;

    uint16_t nth_section_ = 0;

    ast_chapter *chapter_ = nullptr;

    std::vector<std::unique_ptr<ast_subsection>> subsections_;
    std::vector<std::unique_ptr<ast_question>> questions_;
};

class ast_chapter : public ast_node {
   private:
    std::string chapter_;

    uint16_t nth_chapter_;

    std::vector<std::unique_ptr<ast_section>> sections_;
    std::vector<std::unique_ptr<ast_question>> questions_;
};
}  // namespace qac

#endif  // QAC_AST_NODES_H
