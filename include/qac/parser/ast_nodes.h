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

class ast_node {
   public:
    using ptr = std::unique_ptr<ast_node>;

    ast_node(ast_node_enum type) : type_(type) {}

    ast_node_enum type() const { return type_; }

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

    void add_subsection(ast_subsection::ptr subsection) {
        subsections_.push_back(std::move(subsection));
    }

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

   private:
    std::string chapter_;

    uint16_t nth_chapter_;

    section_vector sections_;
};

class ast_root_questions : public ast_node, public has_questions {
   public:
    using ptr = std::unique_ptr<ast_root_questions>;

    ast_root_questions() : ast_node(ast_node_enum::ROOT_QUESTIONS) {}
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

   private:
    chapter_vector chapters_;
};

}  // namespace qac

#endif  // QAC_AST_NODES_H
