#ifndef QAC_PARSER_H
#define QAC_PARSER_H

#include <memory>
#include <vector>
#include <qac/lexer/lexer.h>

namespace qac {

enum class node_enum {
    ROOT,
    QUESTION,
    ANSWER,
    WORD,
    CHAPTER,
    SECTION,
    SUBSECTION,
    LATEX,
    CENTERED_LATEX,
    UNORDERED_LIST,
    UNORDERED_LIST_ITEM,
    ORDERED_LIST,
    ORDERED_LIST_ITEM,
    TABLE,
    BOLD,
    UNDERLINED,
    CODE
};

class node {
   public:
    node(node_enum type) : node(type, "") {}

    node(node_enum type, std::string value) : type_(type), value_(value) {}

    void add_child(std::unique_ptr<node> child) {
        children_.push_back(std::move(child));
    }

    node_enum type() const { return type_; }

    const std::vector<std::unique_ptr<node>> &children() const {
        return children_;
    }

    const std::string &value() { return value_; }

   private:
    node_enum type_;
    std::vector<std::unique_ptr<node>> children_;
    std::string value_;
};

class parser {
   public:
    std::unique_ptr<node> parse(const std::vector<token> &tokens);

   private:
    std::unique_ptr<node> parse(const std::vector<token> &tokens,
                                std::vector<token>::const_iterator &current);
};
}

#endif  // QAC_PARSER_H
