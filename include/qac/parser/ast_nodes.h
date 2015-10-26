#ifndef QAC_AST_NODES_H
#define QAC_AST_NODES_H

#include <memory>
#include <string>
#include <vector>

namespace qac {

class ast_chapter;
class ast_section;
class ast_subsection;

enum class ast_node_enum {
    ROOT_QUESTIONS,
    ROOT_CHAPTERS,
    QUESTION,
    CHAPTER,
    SECTION,
    SUBSECTION
};

class ast_chapter;
class ast_question;
class ast_root_chapters;
class ast_root_questions;
class ast_section;
class ast_subsection;

class ast_visitor {
   public:
    virtual void visit(ast_chapter *node) = 0;
    virtual void visit(ast_question *node) = 0;
    virtual void visit(ast_root_chapters *node) = 0;
    virtual void visit(ast_root_questions *node) = 0;
    virtual void visit(ast_section *node) = 0;
    virtual void visit(ast_subsection *node) = 0;
};

class ast_node {
   public:
    using ptr = std::unique_ptr<ast_node>;

    ast_node(ast_node_enum type) : type_(type) {}

    ast_node_enum type() const { return type_; }

    virtual void accept(ast_visitor &visitor) = 0;

   private:
    ast_node_enum type_;
};

class ast_question : public ast_node {
   public:
    using ptr = std::unique_ptr<ast_question>;

    ast_question(uint16_t nth, const std::string &question,
                 const std::string &answer)
        : ast_node(ast_node_enum::QUESTION),
          nth_question_(nth),
          question_(question),
          answer_(answer) {}

    uint16_t nth_question() const { return nth_question_; }

    const std::string &question() const { return question_; }

    const std::string &answer() const { return answer_; }

    void chapter(ast_chapter *chapter) { chapter_ = chapter; }
    void section(ast_section *section) { section_ = section; }
    void subsection(ast_subsection *subsection) { subsection_ = subsection; }

    virtual void accept(ast_visitor &visitor) override { visitor.visit(this); }

   private:
    std::string question_;
    std::string answer_;

    uint16_t nth_question_ = 0;

    ast_chapter *chapter_ = nullptr;
    ast_section *section_ = nullptr;
    ast_subsection *subsection_ = nullptr;
};

class has_questions {
   public:
    using question_vector = std::vector<ast_question::ptr>;

    void add_question(ast_question::ptr question) {
        questions_.push_back(std::move(question));
    }

    const question_vector &questions() const { return questions_; }

   private:
    question_vector questions_;
};

class ast_subsection : public ast_node, public has_questions {
   public:
    using ptr = std::unique_ptr<ast_subsection>;

    ast_subsection(uint16_t nth, const std::string &subsection)
        : ast_node(ast_node_enum::SUBSECTION),
          nth_subsection_(nth),
          subsection_(subsection) {}

    const std::string &subsection() const { return subsection_; }

    virtual void accept(ast_visitor &visitor) override { visitor.visit(this); }

    uint16_t nth_subsection() const { return nth_subsection_; }

   private:
    std::string subsection_;

    uint16_t nth_subsection_ = 0;

    ast_chapter *chapter_ = nullptr;
    ast_section *section_ = nullptr;
};

class ast_section : public ast_node, public has_questions {
   public:
    using ptr = std::unique_ptr<ast_section>;
    using subsection_vector = std::vector<ast_subsection::ptr>;

    ast_section(uint16_t nth, const std::string &section)
        : ast_node(ast_node_enum::SECTION),
          nth_section_(nth),
          section_(section) {}

    const std::string &section() const { return section_; }

    void add_subsection(ast_subsection::ptr subsection) {
        subsections_.push_back(std::move(subsection));
    }

    const subsection_vector &subsections() const { return subsections_; }

    virtual void accept(ast_visitor &visitor) override { visitor.visit(this); }

    uint16_t nth_section() const { return nth_section_; }

   private:
    std::string section_;

    uint16_t nth_section_ = 0;

    ast_chapter *chapter_ = nullptr;

    subsection_vector subsections_;
};

class ast_chapter : public ast_node, public has_questions {
   public:
    using ptr = std::unique_ptr<ast_chapter>;
    using section_vector = std::vector<ast_section::ptr>;

    ast_chapter(uint16_t nth, const std::string &chapter)
        : ast_node(ast_node_enum::CHAPTER),
          nth_chapter_(nth),
          chapter_(chapter) {}

    uint16_t nth_chapter() const { return nth_chapter_; }

    const std::string &chapter() const { return chapter_; }

    void add_section(ast_section::ptr section) {
        sections_.push_back(std::move(section));
    }

    const section_vector &sections() const { return sections_; }

    virtual void accept(ast_visitor &visitor) override { visitor.visit(this); }

   private:
    std::string chapter_;

    uint16_t nth_chapter_;

    section_vector sections_;
};

class ast_root_questions : public ast_node, public has_questions {
   public:
    using ptr = std::unique_ptr<ast_root_questions>;

    ast_root_questions() : ast_node(ast_node_enum::ROOT_QUESTIONS) {}

    virtual void accept(ast_visitor &visitor) override { visitor.visit(this); }
};

class ast_root_chapters : public ast_node {
   public:
    using ptr = std::unique_ptr<ast_root_chapters>;
    using chapter_vector = std::vector<ast_chapter::ptr>;

    ast_root_chapters() : ast_node(ast_node_enum::ROOT_CHAPTERS) {}

    void add_chapter(ast_chapter::ptr chapter) {
        chapters_.push_back(std::move(chapter));
    }

    const chapter_vector &chapters() const { return chapters_; }

    virtual void accept(ast_visitor &visitor) override { visitor.visit(this); }

   private:
    chapter_vector chapters_;
};

}  // namespace qac

#endif  // QAC_AST_NODES_H
