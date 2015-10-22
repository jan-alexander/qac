#include <qac/parser/cst_to_ast_visitor.h>
#include <qac/generator/generator.h>

#include <glog/logging.h>
#include <string>

using namespace qac;
using namespace std;

std::unique_ptr<ast_node> cst_to_ast_visitor::get_root() { return nullptr; }

void cst_to_ast_visitor::visit(cst_root_questions *node) {
    cout << "entering cst_root_questions" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_root_questions" << endl;
}

void cst_to_ast_visitor::visit(cst_root_chapters *node) {
    cout << "entering cst_root_chapters" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_root_chapters" << endl;
}

void cst_to_ast_visitor::visit(cst_question *node) {
    const std::vector<std::unique_ptr<cst_node>> &children = node->children();
    if (children.size() == 2) {
        texts_stack_.push(ostringstream());
        children[0]->accept(*this);  // question_text
        string question_text = texts_stack_.top().str();
        texts_stack_.pop();

        texts_stack_.push(ostringstream());
        children[1]->accept(*this);  // answer_text
        string answer_text = texts_stack_.top().str();
        texts_stack_.pop();

        cout << "Question: " << question_text << " Answer: " << answer_text
             << endl;
    }
}

void cst_to_ast_visitor::visit(cst_question_text *node) {
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
}

void cst_to_ast_visitor::visit(cst_answer_text *node) {
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
}

void cst_to_ast_visitor::visit(cst_text *node) {
    texts_stack_.top() << node->words() << " ";
}

void cst_to_ast_visitor::visit(cst_latex *node) {
    node->children()[0]->accept(*this);
}

void cst_to_ast_visitor::visit(cst_normal_latex *node) {
    push_text_stream();
    node->children()[0]->accept(*this);
    std::string latex = text_stream().str();
    pop_text_stream();

    generator_->render_normal_latex(text_stream(), trim(latex));
    text_stream() << " ";
}

void cst_to_ast_visitor::visit(cst_centered_latex *node) {
    push_text_stream();
    node->children()[0]->accept(*this);
    std::string latex = text_stream().str();
    pop_text_stream();

    generator_->render_centered_latex(text_stream(), trim(latex));
    text_stream() << " ";
}

void cst_to_ast_visitor::visit(cst_latex_body *node) {
    text_stream() << node->words();
}

void cst_to_ast_visitor::visit(cst_unordered_list *node) {
    push_text_stream();
    node->children()[0]->accept(*this);
    string list_items = text_stream().str();
    pop_text_stream();

    generator_->render_unordered_list(text_stream(), trim(list_items));
}

void cst_to_ast_visitor::visit(cst_unordered_list_item *node) {
    const auto &children = node->children();
    if (children.size() == 2) {
        push_text_stream();
        children[0]->accept(*this);
        string list_item = text_stream().str();
        pop_text_stream();

        generator_->render_unordered_list_item(text_stream(), trim(list_item));
        children[1]->accept(*this);
    }
}

void cst_to_ast_visitor::visit(cst_ordered_list *node) {
    push_text_stream();
    node->children()[0]->accept(*this);
    string list_items = text_stream().str();
    pop_text_stream();

    generator_->render_ordered_list(text_stream(), trim(list_items));
}

void cst_to_ast_visitor::visit(cst_ordered_list_item *node) {
    const auto &children = node->children();
    if (children.size() == 2) {
        push_text_stream();
        children[0]->accept(*this);
        string list_item = text_stream().str();
        pop_text_stream();

        generator_->render_ordered_list_item(text_stream(), trim(list_item));
        children[1]->accept(*this);
    }
}

void cst_to_ast_visitor::visit(cst_list_item_text *node) {
    const std::vector<std::unique_ptr<cst_node>> &children = node->children();
    if (children.size() == 2) {
        push_text_stream();
        children[0]->accept(*this);
        children[1]->accept(*this);
        string list_item_text = text_stream().str();
        pop_text_stream();

        text_stream() << list_item_text;
    }
}

void cst_to_ast_visitor::visit(cst_bold *node) {
    push_text_stream();
    node->children()[0]->accept(*this);
    std::string bold_text = text_stream().str();
    pop_text_stream();

    generator_->render_bold(text_stream(), trim(bold_text));
    text_stream() << " ";
}

void cst_to_ast_visitor::visit(cst_underlined *node) {
    push_text_stream();
    node->children()[0]->accept(*this);
    std::string underlined_text = text_stream().str();
    pop_text_stream();

    generator_->render_underlined(text_stream(), trim(underlined_text));
    text_stream() << " ";
}

void cst_to_ast_visitor::visit(cst_code *node) {
    push_text_stream();
    node->children()[0]->accept(*this);
    std::string code_text = text_stream().str();
    pop_text_stream();

    generator_->render_code(text_stream(), trim(code_text));
    text_stream() << " ";
}

void cst_to_ast_visitor::visit(cst_chapter *node) {
    cout << "entering cst_chapter" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_chapter" << endl;
}

void cst_to_ast_visitor::visit(cst_section *node) {
    cout << "entering cst_section" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_section" << endl;
}

void cst_to_ast_visitor::visit(cst_subsection *node) {
    cout << "entering cst_subsection" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_subsection" << endl;
}

void cst_to_ast_visitor::visit(cst_table *node) {
    cout << "entering cst_table" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_table" << endl;
}

void cst_to_ast_visitor::visit(cst_table_row *node) {
    cout << "entering cst_table_row" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_table_row" << endl;
}

void cst_to_ast_visitor::visit(cst_table_cell *node) {
    cout << "entering cst_table_cell" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_table_cell" << endl;
}
