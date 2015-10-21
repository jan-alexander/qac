#include "qac/generator/html-generator.h"

#include <istream>

using namespace qac;
using namespace std;

string html_generator::get_name() { return "html"; }

string html_generator::get_description() { return "Simple HTML generator"; }

void html_generator::render_bold(std::ostream &os, const std::string &text) {
    os << "<b>" << text << "</b>";
}

void html_generator::render_underlined(std::ostream &os,
                                       const std::string &text) {
    os << "<u>" << text << "</u>";
}

void html_generator::render_code(std::ostream &os, const std::string &text) {
    os << "<code>" << text << "</code>";
}

void html_generator::render_unordered_list_item(std::ostream &os,
                                                const std::string &text) {
    os << "<li>" << text << "<li>";
}

void html_generator::render_ordered_list_item(std::ostream &os,
                                              const std::string &text) {
    render_unordered_list_item(os, text);
}
