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
    node(node_enum type);

private:
    node_enum type_;
};

class parser {
public:
    std::unique_ptr<node> parse(std::vector<token> tokens);
};

}


#endif //QAC_PARSER_H
