#ifndef QAC_CST_NODES_H
#define QAC_CST_NODES_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <boost/algorithm/string/join.hpp>

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
    TABLE_CELL_TEXT,
    IMAGE,
};

std::string to_string(const cst_node_enum &nenum);
std::ostream &operator<<(std::ostream &os, const cst_node_enum &nenum);

class cst_root_questions;
class cst_root_chapters;
class cst_question;
class cst_question_text;
class cst_answer_text;
class cst_text;
class cst_image;
class cst_latex;
class cst_normal_latex;
class cst_centered_latex;
class cst_latex_body;
class cst_unordered_list;
class cst_unordered_list_item;
class cst_ordered_list;
class cst_ordered_list_item;
class cst_list_item_text;
class cst_bold;
class cst_underlined;
class cst_code;
class cst_chapter;
class cst_section;
class cst_subsection;
class cst_table;
class cst_table_row;
class cst_table_cell;
class cst_table_cell_text;

class cst_visitor {
   public:
    virtual void visit(cst_root_questions *node) = 0;
    virtual void visit(cst_root_chapters *node) = 0;
    virtual void visit(cst_question *node) = 0;
    virtual void visit(cst_question_text *node) = 0;
    virtual void visit(cst_answer_text *node) = 0;
    virtual void visit(cst_text *node) = 0;
    virtual void visit(cst_image *node) = 0;
    virtual void visit(cst_latex *node) = 0;
    virtual void visit(cst_normal_latex *node) = 0;
    virtual void visit(cst_centered_latex *node) = 0;
    virtual void visit(cst_latex_body *node) = 0;
    virtual void visit(cst_unordered_list *node) = 0;
    virtual void visit(cst_unordered_list_item *node) = 0;
    virtual void visit(cst_ordered_list *node) = 0;
    virtual void visit(cst_ordered_list_item *node) = 0;
    virtual void visit(cst_list_item_text *node) = 0;
    virtual void visit(cst_bold *node) = 0;
    virtual void visit(cst_underlined *node) = 0;
    virtual void visit(cst_code *node) = 0;
    virtual void visit(cst_chapter *node) = 0;
    virtual void visit(cst_section *node) = 0;
    virtual void visit(cst_subsection *node) = 0;
    virtual void visit(cst_table *node) = 0;
    virtual void visit(cst_table_row *node) = 0;
    virtual void visit(cst_table_cell *node) = 0;
    virtual void visit(cst_table_cell_text *node) = 0;
};

class has_words {
   public:
    void add_word(std::string word) { words_.push_back(word); }
    std::string words() const { return boost::algorithm::join(words_, " "); }

   private:
    std::vector<std::string> words_;
};

class cst_node {
   public:
    cst_node(cst_node_enum type) : cst_node(type, "") {}

    cst_node(cst_node_enum type, std::string value)
        : type_(type), value_(value) {}

    void add_child(std::unique_ptr<cst_node> child) {
        children_.push_back(std::move(child));
    }

    cst_node_enum type() const { return type_; }

    const std::vector<std::unique_ptr<cst_node>> &children() const {
        return children_;
    }

    const std::string &value() { return value_; }

    virtual void accept(cst_visitor &visitor) = 0;

   private:
    cst_node_enum type_;
    std::vector<std::unique_ptr<cst_node>> children_;
    std::string value_;
};

class cst_root_questions : public cst_node {
   public:
    cst_root_questions() : cst_node(cst_node_enum::ROOT) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_root_chapters : public cst_node {
   public:
    cst_root_chapters() : cst_node(cst_node_enum::ROOT) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_question : public cst_node {
   public:
    cst_question() : cst_node(cst_node_enum::QUESTION) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_question_text : public cst_node {
   public:
    cst_question_text() : cst_node(cst_node_enum::QUESTION_TEXT) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_answer_text : public cst_node {
   public:
    cst_answer_text() : cst_node(cst_node_enum::ANSWER_TEXT) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_text : public cst_node, public has_words {
   public:
    cst_text() : cst_node(cst_node_enum::TEXT) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_image : public cst_node {
private:
    int width_ = -1;
    int height_ = -1;
    std::string source_;

public:
    cst_image() : cst_node(cst_node_enum::IMAGE) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }

    void set_source(std::string source) {
        source_ = source;
    }

    void set_source(std::string source, int width, int height) {
        source_ = source;
        width_ = width;
        height_ = height;
    }

    std::string get_source() const {
        return source_;
    }

    int get_width() const {
        return width_;
    }

    int get_height() const {
        return height_;
    }
};

class cst_latex : public cst_node {
   public:
    cst_latex() : cst_node(cst_node_enum::LATEX) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_normal_latex : public cst_node {
   public:
    cst_normal_latex() : cst_node(cst_node_enum::NORMAL_LATEX) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_centered_latex : public cst_node {
   public:
    cst_centered_latex() : cst_node(cst_node_enum::CENTERED_LATEX) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_latex_body : public cst_node, public has_words {
   public:
    cst_latex_body() : cst_node(cst_node_enum::LATEX_BODY) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_unordered_list : public cst_node {
   public:
    cst_unordered_list() : cst_node(cst_node_enum::UNORDERED_LIST) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_unordered_list_item : public cst_node {
   public:
    cst_unordered_list_item() : cst_node(cst_node_enum::UNORDERED_LIST_ITEM) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_ordered_list : public cst_node {
   public:
    cst_ordered_list() : cst_node(cst_node_enum::ORDERED_LIST) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_ordered_list_item : public cst_node {
   public:
    cst_ordered_list_item() : cst_node(cst_node_enum::ORDERED_LIST_ITEM) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_list_item_text : public cst_node {
   public:
    cst_list_item_text() : cst_node(cst_node_enum::LIST_ITEM_TEXT) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_bold : public cst_node {
   public:
    cst_bold() : cst_node(cst_node_enum::BOLD) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_underlined : public cst_node {
   public:
    cst_underlined() : cst_node(cst_node_enum::UNDERLINED) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_code : public cst_node {
   public:
    cst_code() : cst_node(cst_node_enum::CODE) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_chapter : public cst_node {
   public:
    cst_chapter() : cst_node(cst_node_enum::CHAPTER) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_section : public cst_node {
   public:
    cst_section() : cst_node(cst_node_enum::SECTION) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_subsection : public cst_node {
   public:
    cst_subsection() : cst_node(cst_node_enum::SUBSECTION) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_table : public cst_node {
   public:
    cst_table() : cst_node(cst_node_enum::TABLE) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_table_row : public cst_node {
   public:
    cst_table_row() : cst_node(cst_node_enum::TABLE_ROW) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

class cst_table_cell : public cst_node {
   public:
    cst_table_cell() : cst_node(cst_node_enum::TABLE_CELL) {}

    enum class alignment_enum { STANDARD, LEFT, CENTER, RIGHT };

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }

    alignment_enum alignment() const { return alignment_; }

    void alignment(alignment_enum alignment) { alignment_ = alignment; }

   private:
    alignment_enum alignment_ = alignment_enum::STANDARD;
};

class cst_table_cell_text : public cst_node {
   public:
    cst_table_cell_text() : cst_node(cst_node_enum::TABLE_CELL_TEXT) {}

    virtual void accept(cst_visitor &visitor) override { visitor.visit(this); }
};

}  // namespace qac

#endif  // QAC_CST_NODES_H
