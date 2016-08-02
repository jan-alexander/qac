#include "qac/generator/html-generator.h"

#include <iostream>

using namespace qac;
using namespace std;

string html_generator::get_name() { return "html"; }

string html_generator::get_description() { return "Simple HTML generator"; }

void html_generator::render_image(std::ostream &os, const std::string &source,
                          int width, int height) {
    os << "<img src=\"" << source << "\"";
    if (width >= 0 && height >= 0) {
        os << " width=\"" << width << "\" height=\"" << height << "\"";
    }
    os << ">";
}

void html_generator::render_bold(std::ostream &os, const std::string &text) {
    os << "<strong>" << text << "</strong>";
}

void html_generator::render_underlined(std::ostream &os,
                                       const std::string &text) {
    os << "<span class=\"qa_ul\">" << text << "</span>";
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
    if (!caption.empty() && (!questions.empty() || !sections.empty())) {
        os << "<div class=\"qa_chapter\">\n"
           << "    <h1><span>" << FLAGS_chapter << " " << chapter->nth_chapter()
           << "</span>" << caption << "</h1>\n\n" << questions << "\n"
           << sections << "</div>\n\n";
    }
}

void html_generator::render_section(std::ostream &os,
                                    const std::string &caption,
                                    const std::string &questions,
                                    const std::string &subsections,
                                    const ast_section *section) {
    if (!caption.empty() && (!questions.empty() || !subsections.empty())) {
        os << "<div class=\"qa_section\">\n"
           << "    <h2><span>" << FLAGS_section << " " << section->nth_section()
           << "</span>" << caption << "</h2>\n\n" << questions << "\n"
           << subsections << "</div>\n";
    }
}

void html_generator::render_subsection(std::ostream &os,
                                       const std::string &caption,
                                       const std::string &questions,
                                       const ast_subsection *subsection) {
    if (!caption.empty() && !questions.empty()) {
        os << "<div class=\"qa_subsection\">\n"
           << "    <h3><span>" << FLAGS_subsection << " "
           << subsection->nth_subsection() << "</span>" << caption
           << "</h2>\n\n" << questions << "</div>\n";
    }
}

void html_generator::render_question(std::ostream &os,
                                     const std::string &question,
                                     const std::string &answer,
                                     const ast_question *pquestion) {
    os << "<div class=\"qa_question\">\n"
       << "    <h4><span>" << FLAGS_question << " " << pquestion->nth_question()
       << "</span>" << question << "</h4>\n"
       << "    <div class=\"qa_answer\">\n" << answer << "\n    </div>\n"
       << "</div>\n";
}

void html_generator::render_document(std::ostream &os,
                                     const std::string &body) {
    std::string font = "        <link href='https://fonts.googleapis.com/css?family=Open+Sans:400,300,600' rel='stylesheet' type='text/css'>\n";
    std::string mathjax_src = "http://cdn.mathjax.org/mathjax/latest/";

    if (FLAGS_offline) {
        mathjax_src = "MathJax/";
        font = "";
    }

    os << "<!DOCTYPE html>\n"
       << "<html>\n"
       << "    <head>\n"
       << "        <meta charset=\"utf-8\">\n"
       << font
       << "        <title>Q&As</title>\n"
       << "        <script type=\"text/x-mathjax-config\">\n"
       << "            MathJax.Hub.Config({tex2jax: {inlineMath: "
          "[['\\\\(','\\\\)']]}});\n"
       << "        </script>\n"
       << "        <script type=\"text/javascript\" "
          "src=\"" << mathjax_src <<
          "MathJax.js?config=TeX-AMS-MML_HTMLorMML\">\n"
       << "        </script>\n"
       << "        <style>\n"
       << "            body {\n"
       << "                    font-family: 'Open Sans', sans-serif;\n"
       << "                    color: black;\n"
       << "                    background-color: aliceblue;\n"
       << "                    margin: 0;\n"
       << "                    padding: 0;\n"
       << "            }\n"
       << "            h1, h2, h3, h4 {\n"
       << "                    padding: 10px;\n"
       << "                    font-weight: 400;\n"
       << "            }\n"
       << "            h1 > span:first-child, h2 > span:first-child, h3 > "
          "span:first-child, h4 > span:first-child {\n"
       << "                    color: lightgrey;\n"
       << "                    font-weight: 300;\n"
       << "                    padding-right: .5em;\n"
       << "            }\n"
       << "            h1, h2, h3 {\n"
       << "                    background-image: linear-gradient(to bottom, "
          "aliceblue 0%, white 100%);\n"
       << "            }\n"
       << "            h4 {\n"
       << "                    background-image: linear-gradient(to bottom, "
          "aliceblue 0%, #FFF 100%);\n"
       << "            }\n"
       << "            h2, h3, h4 {\n"
       << "                    border-bottom-left-radius: 1em;\n"
       << "            }\n"
       << "                    .qa_question, .qa_section, .qa_subsection {\n"
       << "                    margin-left: 50px;\n"
       << "            }\n"
       << "                    .qa_question div.qa_answer {\n"
       << "                padding: 10px;\n"
       << "                margin: 50px;\n"
       << "                margin-top: 0;\n"
       << "                background-color: white;\n"
       << "                border-radius: .5em;\n"
       << "                box-shadow: 0 0 1em #EEE;\n"
       << "            }\n"
       << "            .qa_ul {\n"
       << "                text-decoration: underline;\n"
       << "            }\n"
       << "            table {\n"
       << "                border-collapse: collapse;\n"
       << "            }\n"
       << "            table, th, td {\n"
       << "                border: 1px solid black;\n"
       << "            }\n"
       << "            td {\n"
       << "                padding: 1em;\n"
       << "            }\n"
       << "            td.qa_la {\n"
       << "                text-align: left;\n"
       << "            }\n"
       << "            td.qa_ca {\n"
       << "                text-align: center;\n"
       << "            }\n"
       << "            td.qa_ra {\n"
       << "                text-align: right;\n"
       << "            }\n"
       << "        </style>\n"
       << "    </head>\n"
       << "    <body>\n" << body << "    </body>"
       << "</html>";
}

void html_generator::render_table_cell(std::ostream &os,
                                       const std::string &cell_body) {
    os << "<td>" << cell_body << "</td>";
}

void html_generator::render_table_cell_left_aligned(
    std::ostream &os, const std::string &cell_body) {
    os << "<td class=\"qa_la\">" << cell_body << "</td>";
}

void html_generator::render_table_cell_right_aligned(
    std::ostream &os, const std::string &cell_body) {
    os << "<td class=\"qa_ra\">" << cell_body << "</td>";
}

void html_generator::render_table_cell_center_aligned(
    std::ostream &os, const std::string &cell_body) {
    os << "<td class=\"qa_ca\">" << cell_body << "</td>";
}

void html_generator::render_table_row(std::ostream &os,
                                      const std::string &cells) {
    os << "<tr>" << cells << "</tr>";
}

void html_generator::render_table(std::ostream &os, const std::string &rows) {
    os << "<table>" << rows << "</table>";
}
