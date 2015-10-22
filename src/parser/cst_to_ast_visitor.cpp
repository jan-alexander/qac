#include <qac/parser/cst_to_ast_visitor.h>
#include <qac/generator/generator.h>

#include <glog/logging.h>
#include <string>

using namespace qac;
using namespace std;

std::unique_ptr<ast_node> cst_to_ast_visitor::get_root() { return nullptr; }

std::string cst_to_ast_visitor::text() {
    std::string text = text_.str();
    text_.str("");
    text_.clear();
    return text;
}

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
    // has 3 or 0 children:
    //  - question_text
    //  - answer_text
    //  - question
    cout << "entering cst_question: size=" << node->children().size() << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_question" << endl;

    // const std::vector<std::unique_ptr<cst_node>> children = node->children();
    // if (children.size() == 2) {
    // children[0]->accept(*this); // question_text
    // string question_text = text();

    // children[1]->accept(*this); // answer_text
    // string answer_text = text();

    // cout << "Question: " << question_text << " Answer: " << answer_text <<
    // endl;
    //}
}

void cst_to_ast_visitor::visit(cst_question_text *node) {
    cout << "entering cst_question_text" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_question_text" << endl;
}

void cst_to_ast_visitor::visit(cst_answer_text *node) {
    cout << "entering cst_answer_text" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_answer_text" << endl;
}

void cst_to_ast_visitor::visit(cst_text *node) {
    cout << "entering cst_text" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_text" << endl;
}

void cst_to_ast_visitor::visit(cst_latex *node) {
    cout << "entering cst_latex" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_latex" << endl;
}

void cst_to_ast_visitor::visit(cst_normal_latex *node) {
    cout << "entering cst_normal_latex" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_normal_latex" << endl;
}

void cst_to_ast_visitor::visit(cst_centered_latex *node) {
    cout << "entering cst_centered_latex" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_centered_latex" << endl;
}

void cst_to_ast_visitor::visit(cst_latex_body *node) {
    cout << "entering cst_latex_body" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_latex_body" << endl;
}

void cst_to_ast_visitor::visit(cst_unordered_list *node) {
    cout << "entering cst_unordered_list" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_unordered_list" << endl;
}

void cst_to_ast_visitor::visit(cst_unordered_list_item *node) {
    cout << "entering cst_unordered_list_item" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_unordered_list_item" << endl;
}

void cst_to_ast_visitor::visit(cst_ordered_list *node) {
    cout << "entering cst_ordered_list" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_ordered_list" << endl;
}

void cst_to_ast_visitor::visit(cst_ordered_list_item *node) {
    cout << "entering cst_ordered_list_item" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_ordered_list_item" << endl;
}

void cst_to_ast_visitor::visit(cst_list_item_text *node) {
    cout << "entering cst_list_item_text" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_list_item_text" << endl;
}

void cst_to_ast_visitor::visit(cst_bold *node) {
    cout << "entering cst_bold" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_bold" << endl;
}

void cst_to_ast_visitor::visit(cst_underlined *node) {
    cout << "entering cst_underlined" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_underlined" << endl;
}

void cst_to_ast_visitor::visit(cst_code *node) {
    cout << "entering cst_code" << endl;
    for (const auto &child : node->children()) {
        child->accept(*this);
    }
    cout << "exiting cst_code" << endl;
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
