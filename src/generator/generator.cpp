#include <qac/generator/generator.h>
#include <qac/parser/cst_to_ast_visitor.h>

#include <iostream>

using namespace std;
using namespace qac;

void generator::generate(cst_node *root, std::ostream &os) {
    os << "generator::generate" << endl;

    cst_to_ast_visitor visitor(this);

    root->accept(visitor);

    auto ast_root = visitor.root();
}

void generator::render_normal_latex(std::ostream &os, const std::string &text) {
    os << "\\(" << text << "\\)";
}

void generator::render_centered_latex(std::ostream &os,
                                      const std::string &text) {
    os << "\\[" << text << "\\]";
}
