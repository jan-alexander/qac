#ifndef QAC_CST_NODES_H
#define QAC_CST_NODES_H

namespace qac {

enum class cst_node_enum {
    ROOT,
    QUESTION,
    QUESTION_TEXT,
    ANSWER_TEXT,
    TEXT,
    LATEX,
    NORMAL_LATEX,
    CENTERED_LATEX,
    LATEX_BODY,
    UNORDERED_LIST,
    UNORDERED_LIST_ITEM,
    ORDERED_LIST,
    ORDERED_LIST_ITEM,
    LIST_ITEM_TEXT,
    BOLD,
    UNDERLINED,
    CODE,
    CHAPTER,
    SECTION,
    SUBSECTION,
    TABLE,
    TABLE_ROW,
    TABLE_CELL,
};

std::string to_string(const cst_node_enum &nenum);
std::ostream &operator<<(std::ostream &os, const cst_node_enum &nenum);

class cst_node {
   public:
    cst_node(cst_node_enum type) : cst_node(type, "") {}

    cst_node(cst_node_enum type, std::string value) : type_(type), value_(value) {}

    void add_child(std::unique_ptr<cst_node> child) {
        children_.push_back(std::move(child));
    }

    cst_node_enum type() const { return type_; }

    const std::vector<std::unique_ptr<cst_node>> &children() const {
        return children_;
    }

    const std::string &value() { return value_; }

   private:
    cst_node_enum type_;
    std::vector<std::unique_ptr<cst_node>> children_;
    std::string value_;
};
}  // namespace qac

#endif  // QAC_CST_NODES_H
