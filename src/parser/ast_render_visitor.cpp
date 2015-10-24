#include <qac/parser/ast_render_visitor.h>
#include <qac/generator/generator.h>

#include <glog/logging.h>

using namespace qac;
using namespace std;

void ast_render_visitor::visit(ast_chapter *node) {
    DLOG_IF(INFO, LOG_VISIT)
        << "entering ast_chapter questions: " << node->questions().size()
        << " sections: " << node->sections().size();

    push_text_stream();
    for (const auto &question : node->questions()) {
        question->accept(*this);
    }
    std::string questions = text_stream().str();
    pop_text_stream();

    push_text_stream();
    for (const auto &section : node->sections()) {
        section->accept(*this);
    }
    std::string sections = text_stream().str();
    pop_text_stream();

    generator_->render_chapter(text_stream(), node->chapter(), questions,
                               sections, node);

    DLOG_IF(INFO, LOG_VISIT) << "exiting ast_chapter";
}

void ast_render_visitor::visit(ast_question *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering ast_question";

    generator_->render_question(text_stream(), node->question(), node->answer(),
                                node);

    DLOG_IF(INFO, LOG_VISIT) << "exiting ast_question";
}

void ast_render_visitor::visit(ast_root_chapters *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering ast_root_chapter chapters: "
                             << node->chapters().size();

    push_text_stream();
    for (const auto &chapter : node->chapters()) {
        chapter->accept(*this);
    }
    std::string body = text_stream().str();
    pop_text_stream();

    push_text_stream();
    generator_->render_document(text_stream(), body);
    rendered_qa_ = text_stream().str();
    pop_text_stream();

    DLOG_IF(INFO, LOG_VISIT) << "exiting ast_root_chapter";
}

void ast_render_visitor::visit(ast_root_questions *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering ast_root_questions questions: "
                             << node->questions().size();

    push_text_stream();
    for (const auto &question : node->questions()) {
        question->accept(*this);
    }
    std::string body = text_stream().str();
    pop_text_stream();

    push_text_stream();
    generator_->render_document(text_stream(), body);
    rendered_qa_ = text_stream().str();
    pop_text_stream();

    DLOG_IF(INFO, LOG_VISIT) << "exiting ast_root_questions";
}

void ast_render_visitor::visit(ast_section *node) {
    DLOG_IF(INFO, LOG_VISIT)
        << "entering ast_section questions: " << node->questions().size()
        << " subsections: " << node->subsections().size();

    push_text_stream();
    for (const auto &question : node->questions()) {
        question->accept(*this);
    }
    std::string questions = text_stream().str();
    pop_text_stream();

    push_text_stream();
    for (const auto &subsection : node->subsections()) {
        subsection->accept(*this);
    }
    std::string subsections = text_stream().str();
    pop_text_stream();

    generator_->render_section(text_stream(), node->section(), questions,
                               subsections, node);

    DLOG_IF(INFO, LOG_VISIT) << "exiting ast_section";
}

void ast_render_visitor::visit(ast_subsection *node) {
    DLOG_IF(INFO, LOG_VISIT) << "entering ast_subsection questions: "
                             << node->questions().size();

    push_text_stream();
    for (const auto &question : node->questions()) {
        question->accept(*this);
    }
    std::string questions = text_stream().str();
    pop_text_stream();

    generator_->render_subsection(text_stream(), node->subsection(), questions,
                                  node);

    DLOG_IF(INFO, LOG_VISIT) << "exiting ast_subsection";
}
