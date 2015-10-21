#include "qac/generator/generator.h"

#include <iostream>

using namespace std;
using namespace qac;

void generator::generate(cst_node *root, std::ostream &os) {
    os << "generator::generate" << endl;
}

void generator::render_normal_latex(std::ostream &os, const std::string &text) {
    os << "\\(" << text << "\\)";
}

void generator::render_centered_latex(std::ostream &os, const std::string &text) {
    os << "\\[" << text << "\\]";
}
