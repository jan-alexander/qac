#include <qac/generator/generator.h>
#include <qac/parser/cst_to_ast_visitor.h>
#include <qac/parser/ast_render_visitor.h>

#include <iostream>

using namespace std;
using namespace qac;

void generator::generate(cst_node *cst_root, std::ostream &os) {
    cst_to_ast_visitor converter(this);
    cst_root->accept(converter);
    auto ast_root = converter.root();

    ast_render_visitor renderer(this);
    ast_root->accept(renderer);
    os << renderer.rendered_qa();
}

void generator::render_normal_latex(std::ostream &os, const std::string &text) {
    os << "\\(" << text << "\\)";
}

void generator::render_centered_latex(std::ostream &os,
                                      const std::string &text) {
    os << "\\[" << text << "\\]";
}
