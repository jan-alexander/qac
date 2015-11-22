#include <qac/parser/cst_to_ast_visitor.h>
#include <qac/generator/generator.h>

#include <glog/logging.h>
#include <string>

using namespace qac;
using namespace std;

void cst_to_ast_visitor::visit(cst_root_questions *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_root_questions size: "
                             << node->children().size();

    root_ = make_unique<ast_root_questions>();
    node->children()[0]->accept(*this);

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_root_questions";
}

void cst_to_ast_visitor::visit(cst_root_chapters *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_root_chapters size: "
                             << node->children().size();

    root_ = make_unique<ast_root_chapters>();
    node->children()[0]->accept(*this);

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_root_chapters";
}

void cst_to_ast_visitor::visit(cst_question *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_question size: "
                             << node->children().size();

    const std::vector<std::unique_ptr<cst_node>> &children = node->children();
    if (children.size() == 3) {
        texts_stack_.push(ostringstream());
        children[0]->accept(*this);  // question_text
        string question_text = texts_stack_.top().str();
        texts_stack_.pop();

        texts_stack_.push(ostringstream());
        children[1]->accept(*this);  // answer_text
        string answer_text = texts_stack_.top().str();
        texts_stack_.pop();

        ast_question::ptr question = make_unique<ast_question>(
            ++nth_question_, question_text, answer_text);

        question->chapter(chapter_.get());
        question->section(section_.get());
        question->subsection(subsection_.get());

        switch (state_) {
            case cst_to_ast_visitor_state::IN_ROOT:
                reinterpret_cast<ast_root_questions *>(root_.get())
                    ->add_question(std::move(question));
                break;

            case cst_to_ast_visitor_state::IN_CHAPTER:
                chapter_->add_question(std::move(question));
                break;

            case cst_to_ast_visitor_state::IN_SECTION:
                section_->add_question(std::move(question));
                break;

            case cst_to_ast_visitor_state::IN_SUBSECTION:
                subsection_->add_question(std::move(question));
                break;
        }

        DLOG_IF(INFO, LOG_QUESTION) << nth_chapter_ << "-" << nth_section_
                                    << "-" << nth_subsection_ << " Question "
                                    << nth_question_ << ": " << question_text
                                    << " Answer: " << answer_text;

        children[2]->accept(*this);  // following questions
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_question";
}

void cst_to_ast_visitor::visit(cst_question_text *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_question_text size: "
                             << node->children().size();

    for (const auto &child : node->children()) {
        child->accept(*this);
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_question_text";
}

void cst_to_ast_visitor::visit(cst_answer_text *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_answer_text size: "
                             << node->children().size();

    for (const auto &child : node->children()) {
        child->accept(*this);
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_answer_text";
}

void cst_to_ast_visitor::visit(cst_text *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_text size: "
                             << node->children().size();

    texts_stack_.top() << node->words() << " ";

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_text";
}

void cst_to_ast_visitor::visit(cst_image *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_image size: "
                             << node->children().size();

    generator_->render_image(text_stream(), node->get_source(),
                             node->get_width(), node->get_height());
    text_stream() << " ";

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_image";
}

void cst_to_ast_visitor::visit(cst_latex *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_latex size: "
                             << node->children().size();

    node->children()[0]->accept(*this);

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_latex";
}

void cst_to_ast_visitor::visit(cst_normal_latex *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_normal_latex size: "
                             << node->children().size();

    push_text_stream();
    node->children()[0]->accept(*this);
    std::string latex = text_stream().str();
    pop_text_stream();

    generator_->render_normal_latex(text_stream(), trim(latex));
    text_stream() << " ";

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_normal_latex";
}

void cst_to_ast_visitor::visit(cst_centered_latex *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_centered_latex size: "
                             << node->children().size();

    push_text_stream();
    node->children()[0]->accept(*this);
    std::string latex = text_stream().str();
    pop_text_stream();

    generator_->render_centered_latex(text_stream(), trim(latex));
    text_stream() << " ";

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_centered_latex";
}

void cst_to_ast_visitor::visit(cst_latex_body *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_latex_body size: "
                             << node->children().size();

    text_stream() << node->words();

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_latex_body";
}

void cst_to_ast_visitor::visit(cst_unordered_list *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_unordered_list size: "
                             << node->children().size();

    push_text_stream();
    node->children()[0]->accept(*this);
    string list_items = text_stream().str();
    pop_text_stream();

    generator_->render_unordered_list(text_stream(), trim(list_items));

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_unordered_list";
}

void cst_to_ast_visitor::visit(cst_unordered_list_item *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_unordered_list_item size: "
                             << node->children().size();

    const auto &children = node->children();
    if (children.size() == 2) {
        push_text_stream();
        children[0]->accept(*this);
        string list_item = text_stream().str();
        pop_text_stream();

        generator_->render_unordered_list_item(text_stream(), trim(list_item));
        children[1]->accept(*this);
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_unordered_list_item";
}

void cst_to_ast_visitor::visit(cst_ordered_list *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_ordered_list size: "
                             << node->children().size();

    push_text_stream();
    node->children()[0]->accept(*this);
    string list_items = text_stream().str();
    pop_text_stream();

    generator_->render_ordered_list(text_stream(), trim(list_items));

    DLOG_IF(INFO, LOG_VISIT) << "entering cst_ordered_list";
}

void cst_to_ast_visitor::visit(cst_ordered_list_item *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_ordered_list_item size: "
                             << node->children().size();

    const auto &children = node->children();
    if (children.size() == 2) {
        push_text_stream();
        children[0]->accept(*this);
        string list_item = text_stream().str();
        pop_text_stream();

        generator_->render_ordered_list_item(text_stream(), trim(list_item));
        children[1]->accept(*this);
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_ordered_list_item";
}

void cst_to_ast_visitor::visit(cst_list_item_text *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_list_item_text size: "
                             << node->children().size();

    const std::vector<std::unique_ptr<cst_node>> &children = node->children();
    if (children.size() == 2) {
        push_text_stream();
        children[0]->accept(*this);
        children[1]->accept(*this);
        string list_item_text = text_stream().str();
        pop_text_stream();

        text_stream() << list_item_text;
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_list_item_text";
}

void cst_to_ast_visitor::visit(cst_bold *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_bold size: "
                             << node->children().size();

    push_text_stream();
    node->children()[0]->accept(*this);
    std::string bold_text = text_stream().str();
    pop_text_stream();

    generator_->render_bold(text_stream(), trim(bold_text));
    text_stream() << " ";

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_bold";
}

void cst_to_ast_visitor::visit(cst_underlined *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_underlined size: "
                             << node->children().size();

    push_text_stream();
    node->children()[0]->accept(*this);
    std::string underlined_text = text_stream().str();
    pop_text_stream();

    generator_->render_underlined(text_stream(), trim(underlined_text));
    text_stream() << " ";

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_underlined";
}

void cst_to_ast_visitor::visit(cst_code *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_code size: "
                             << node->children().size();

    push_text_stream();
    node->children()[0]->accept(*this);
    std::string code_text = text_stream().str();
    pop_text_stream();

    generator_->render_code(text_stream(), trim(code_text));
    text_stream() << " ";

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_code";
}

void cst_to_ast_visitor::visit(cst_chapter *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_chapter size: "
                             << node->children().size();

    const auto &children = node->children();
    if (children.size() == 4) {
        push_text_stream();
        children[0]->accept(*this);  // caption
        std::string caption = text_stream().str();
        pop_text_stream();

        state_ = cst_to_ast_visitor_state::IN_CHAPTER;
        chapter_ = make_unique<ast_chapter>(++nth_chapter_, caption);
        nth_section_ = 0;
        nth_subsection_ = 0;

        DLOG_IF(INFO, LOG_CHAPTER) << "Chapter " << nth_chapter_ << ": "
                                   << caption;

        children[1]->accept(*this);  // questions
        children[2]->accept(*this);  // sections

        reinterpret_cast<ast_root_chapters *>(root_.get())
            ->add_chapter(std::move(chapter_));
        chapter_.reset();

        children[3]->accept(*this);  // following chapters
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_chapter";
}

void cst_to_ast_visitor::visit(cst_section *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_section size: "
                             << node->children().size();

    const auto &children = node->children();
    if (children.size() == 4) {
        push_text_stream();
        children[0]->accept(*this);  // caption
        std::string caption = text_stream().str();
        pop_text_stream();

        state_ = cst_to_ast_visitor_state::IN_SECTION;
        section_ = make_unique<ast_section>(++nth_section_, caption);
        nth_subsection_ = 0;

        DLOG_IF(INFO, LOG_SECTION) << nth_chapter_ << " Section "
                                   << nth_section_ << ": " << caption;

        children[1]->accept(*this);  // questions
        children[2]->accept(*this);  // subsections

        chapter_->add_section(std::move(section_));
        section_.reset();

        children[3]->accept(*this);  // following sections
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_section";
}

void cst_to_ast_visitor::visit(cst_subsection *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_subsection size: "
                             << node->children().size();

    const auto &children = node->children();
    if (children.size() == 3) {
        push_text_stream();
        children[0]->accept(*this);  // caption
        std::string caption = text_stream().str();
        pop_text_stream();

        state_ = cst_to_ast_visitor_state::IN_SUBSECTION;
        subsection_ = make_unique<ast_subsection>(++nth_subsection_, caption);

        DLOG_IF(INFO, LOG_SUBSECTION) << nth_chapter_ << "-" << nth_section_
                                      << " Subsection " << nth_subsection_
                                      << ": " << caption;

        children[1]->accept(*this);  // questions

        section_->add_subsection(std::move(subsection_));
        subsection_.reset();

        children[2]->accept(*this);  // following subsections
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_subsection";
}

void cst_to_ast_visitor::visit(cst_table *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_table size: "
                             << node->children().size();

    const auto &children = node->children();
    if (children.size() == 1) {
        push_text_stream();
        children[0]->accept(*this);  // 0: row
        string rows = text_stream().str();
        pop_text_stream();

        generator_->render_table(text_stream(), trim(rows));
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_table";
}

void cst_to_ast_visitor::visit(cst_table_row *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_table_row size: "
                             << node->children().size();

    const auto &children = node->children();
    if (children.size() == 2) {
        push_text_stream();
        children[0]->accept(*this);  // 0: cell
        string cells = text_stream().str();
        pop_text_stream();

        generator_->render_table_row(text_stream(), trim(cells));

        children[1]->accept(*this);  // 1: row
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_table_row";
}

void cst_to_ast_visitor::visit(cst_table_cell *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_table_cell size: "
                             << node->children().size();

    const auto &children = node->children();
    if (children.size() == 2) {
        push_text_stream();
        children[0]->accept(*this);  // 0: cell text
        string cell_text = text_stream().str();
        pop_text_stream();

        switch (node->alignment()) {
            case cst_table_cell::alignment_enum::STANDARD:
                generator_->render_table_cell(text_stream(), trim(cell_text));
                break;

            case cst_table_cell::alignment_enum::LEFT:
                generator_->render_table_cell_left_aligned(text_stream(),
                                                           trim(cell_text));
                break;

            case cst_table_cell::alignment_enum::CENTER:
                generator_->render_table_cell_center_aligned(text_stream(),
                                                             trim(cell_text));
                break;

            case cst_table_cell::alignment_enum::RIGHT:
                generator_->render_table_cell_right_aligned(text_stream(),
                                                            trim(cell_text));
                break;
        }

        children[1]->accept(*this);  // 1: cell
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_table_cell";
}

void cst_to_ast_visitor::visit(cst_table_cell_text *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering cst_table_cell_text size: "
                             << node->children().size();

    const auto &children = node->children();
    if (children.size() == 2) {
        push_text_stream();
        children[0]->accept(*this);  // 0: (text/...)
        children[1]->accept(*this);  // 1: cell text
        string cell_text = text_stream().str();
        pop_text_stream();

        text_stream() << cell_text;
    }

    DLOG_IF(INFO, LOG_VISIT) << "exiting cst_table_cell_text";
}
