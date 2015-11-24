#include <qac/generator/anki-generator.h>

#include <iostream>

#include <boost/algorithm/string.hpp>

using namespace qac;
using namespace std;

string anki_generator::get_name() { return "anki"; }

string anki_generator::get_description() { return "Anki text file generator"; }

void anki_generator::render_image(std::ostream &os, const std::string &source,
                                  int width, int height) {
    // strip the folder from the source path
    std::string::size_type sep_pos = source.rfind('/');
    html_generator::render_image(
        os, sep_pos == std::string::npos ? source : source.substr(sep_pos + 1),
        width, height);
}

void anki_generator::render_normal_latex(std::ostream &os,
                                         const std::string &text) {
    os << "[$]" << text << "[/$]";
}

void anki_generator::render_centered_latex(std::ostream &os,
                                           const std::string &text) {
    os << "[$$]" << text << "[/$$]";
}

void anki_generator::render_underlined(std::ostream &os,
                                       const std::string &text) {
    os << "<span style=\"text-decoration: underline\">" << text << "</span>";
}

void anki_generator::render_chapter(std::ostream &os,
                                    const std::string &caption,
                                    const std::string &questions,
                                    const std::string &sections,
                                    const ast_chapter *chapter) {
    os << questions << sections;
}

void anki_generator::render_section(std::ostream &os,
                                    const std::string &caption,
                                    const std::string &questions,
                                    const std::string &subsections,
                                    const ast_section *section) {
    os << questions << subsections;
}

void anki_generator::render_subsection(std::ostream &os,
                                       const std::string &caption,
                                       const std::string &questions,
                                       const ast_subsection *subsection) {
    os << questions;
}

void anki_generator::render_question(std::ostream &os,
                                     const std::string &question,
                                     const std::string &answer,
                                     const ast_question *pquestion) {
    vector<string> tags;

    if (pquestion->has_chapter()) {
        tags.push_back(FLAGS_chapter + " " +
                       std::to_string(pquestion->nth_chapter()));
        tags.push_back(pquestion->chapter());

        if (pquestion->has_section()) {
            tags.push_back(FLAGS_section + " " +
                           std::to_string(pquestion->nth_section()));
            tags.push_back(pquestion->section());

            if (pquestion->has_subsection()) {
                tags.push_back(FLAGS_subsection + " " +
                               std::to_string(pquestion->nth_subsection()));
                tags.push_back(pquestion->subsection());
            }
        }
    }

    string tag_string;
    if (!tags.empty()) {
        for (auto &tag : tags) {
            boost::trim(tag);
            boost::replace_all(tag, " ", "_");
        }
        tag_string = boost::algorithm::join(tags, " ");
    }

    os << question << "\t" << answer << "\t" << tag_string << "\n";
}

void anki_generator::render_document(std::ostream &os,
                                     const std::string &body) {
    os << body;
}

void anki_generator::render_table(std::ostream &os, const std::string &rows) {
    os << "<table style=\"border: 1px solid black; border-collapse: collapse\">"
       << rows << "</table>";
}

void anki_generator::render_table_cell(std::ostream &os,
                                       const std::string &cell_body) {
    os << "<td style=\"border: 1px solid black; padding: 1em\">" << cell_body
       << "</td>";
}

void anki_generator::render_table_cell_left_aligned(
    std::ostream &os, const std::string &cell_body) {
    os << "<td style=\"border: 1px solid black; text-align: left; padding: "
          "1em\">"
       << cell_body << "</td>";
}

void anki_generator::render_table_cell_right_aligned(
    std::ostream &os, const std::string &cell_body) {
    os << "<td style=\"border: 1px solid black; text-align: right; padding: "
          "1em\">"
       << cell_body << "</td>";
}

void anki_generator::render_table_cell_center_aligned(
    std::ostream &os, const std::string &cell_body) {
    os << "<td style=\"border: 1px solid black; text-align: center; padding: "
          "1em\">"
       << cell_body << "</td>";
}
