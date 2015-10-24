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

void html_generator::render_unordered_list(std::ostream &os,
                                           const std::string &text) {
    os << "<ul>" << text << "</ul>";
}

void html_generator::render_unordered_list_item(std::ostream &os,
                                                const std::string &text) {
    os << "<li>" << text << "</li>";
}

void html_generator::render_ordered_list(std::ostream &os,
                                         const std::string &text) {
    os << "<ol>" << text << "</ol>";
}

void html_generator::render_ordered_list_item(std::ostream &os,
                                              const std::string &text) {
    render_unordered_list_item(os, text);
}

void html_generator::render_chapter(std::ostream &os,
                                    const std::string &caption,
                                    const std::string &questions,
                                    const std::string &sections,
                                    const ast_chapter *chapter) {
    os << "Chapter: " << caption << "\nQuestions: \n" << questions
       << "\n\n Sections:\n" << sections << "\n";
}

void html_generator::render_section(std::ostream &os,
                                    const std::string &caption,
                                    const std::string &questions,
                                    const std::string &subsections,
                                    const ast_section *section) {
    os << "Section: " << caption << "\n" << questions
       << "\n Subsections: " << subsections << "\n\n";
}

void html_generator::render_subsection(std::ostream &os,
                                       const std::string &caption,
                                       const std::string &questions,
                                       const ast_subsection *subsection) {
    os << "Subsection: " << caption << "\n" << questions << "\n\n";
}

void html_generator::render_question(std::ostream &os,
                                     const std::string &question,
                                     const std::string &answer,
                                     const ast_question *pquestion) {
    os << "Question: " << question << " Answer: " << answer << "\n";
}

void html_generator::render_document(std::ostream &os,
                                     const std::string &body) {
    os << "header\n" << body << "\nfooter\n";
}
