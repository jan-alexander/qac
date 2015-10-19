#include <iostream>
#include <map>

#include "qac/generator/html-generator.h"
#include "qac/lexer/lexer.h"
#include "qac/parser/parser.h"

using namespace qac;
using namespace std;

#define TOKEN(t)        \
    case token_enum::t: \
        return #t
std::string to_string(token_enum tenum) {
    switch (tenum) {
        TOKEN(QUESTION);
        TOKEN(ANSWER);
        TOKEN(WORD);
        TOKEN(LATEX_OPENING);
        TOKEN(LATEX_CENTERED_OPENING);
        TOKEN(LATEX_CODE);
        TOKEN(LATEX_CLOSING);
        TOKEN(LATEX_CENTERED_CLOSING);
        TOKEN(NEW_LINE);
        TOKEN(EMPTY_LINE);
        TOKEN(UNORDERED_LIST_ITEM);
        TOKEN(ORDERED_LIST_ITEM);
        TOKEN(TABLE_CELL);
        TOKEN(TABLE_CELL_LEFT_ALIGNED);
        TOKEN(TABLE_CELL_RIGHT_ALIGNED);
        TOKEN(TABLE_CELL_CENTER_ALIGNED);
        TOKEN(TABLE_DIVIDER);
        TOKEN(BOLD_OPENING);
        TOKEN(BOLD_CLOSING);
        TOKEN(UNDERLINE_OPENING);
        TOKEN(UNDERLINE_CLOSING);
        TOKEN(CHAPTER);
        TOKEN(SECTION);
        TOKEN(SUBSECTION);
        TOKEN(CODE_OPENING);
        TOKEN(CODE_CLOSING);
    }
}
#undef TOKEN

string to_string(node_enum nenum) {
    switch (nenum) {
        case node_enum::ROOT:
            return "ROOT";
        case node_enum::QUESTION:
            return "QUESTION";
        case node_enum::QUESTION_TEXT:
            return "QUESTION_TEXT";
        case node_enum::ANSWER_TEXT:
            return "ANSWER_TEXT";
        case node_enum::TEXT:
            return "TEXT";
        case node_enum::LATEX:
            return "LATEX";
        case node_enum::NORMAL_LATEX:
            return "NORMAL_LATEX";
        case node_enum::CENTERED_LATEX:
            return "CENTERED_LATEX";
        case node_enum::LATEX_BODY:
            return "LATEX_BODY";
        case node_enum::UNORDERED_LIST:
            return "UNORDERED_LIST";
        case node_enum::UNORDERED_LIST_ITEM:
            return "UNORDERED_LIST_ITEM";
        case node_enum::ORDERED_LIST:
            return "ORDERED_LIST";
        case node_enum::ORDERED_LIST_ITEM:
            return "ORDERED_LIST_ITEM";
        case node_enum::LIST_ITEM_TEXT:
            return "LIST_ITEM_TEXT";
        case node_enum::BOLD:
            return "BOLD";
        case node_enum::UNDERLINED:
            return "UNDERLINED";
        case node_enum::CODE:
            return "CODE";
        case node_enum::CHAPTER:
            return "CHAPTER";
        case node_enum::SECTION:
            return "SECTION";
        case node_enum::SUBSECTION:
            return "SUBSECTION";
        case node_enum::TABLE:
            return "TABLE";
        case node_enum::TABLE_ROW:
            return "TABLE_ROW";
        case node_enum::TABLE_CELL:
            return "TABLE_CELL";
        default:
            return ">>UNKNOWN<<";
    }
}

void print_token(const vector<token> &tokens) {
    for (token token : tokens) {
        cout << "t: " << to_string(token.get_token()) << " "
             << token.get_value() << endl;
    }
}

void print_ast(const node *node, int level = 0, bool last = false,
               bool l0last = false) {
    static const string pipe = "│";
    static const string mux = "├";
    static const string lmux = "└";
    static const char nl = '\n';

    if (level) {
        for (int i = 0; i < level - 1; ++i) {
            cout << (l0last ? " " : pipe) << " ";
        }
        cout << (last ? lmux : mux) << "─";
    }

    const auto &children = node->children();
    int nr_children = children.size();
    cout << (nr_children ? "┬" : "") << to_string(node->type()) << nl;

    if (nr_children) {
        for (int i = 0; i < nr_children - 1; ++i) {
            print_ast(children[i].get(), level + 1);
        }
        print_ast(children[nr_children - 1].get(), level + 1, true,
                  l0last || level == 0);
    }
}

int main(int argc, const char *argv[]) {
    if (argc < 3) {
        string exe(argv[0]);

        cerr << "Usage: "
             << exe.substr(exe.rfind('/') + 1) +
                    " <generator> <qa-file> [<output>]\n";
        return -1;
    }

    const char *generator = argv[1];
    const char *input_file = argv[2];
    const char *output_file = (argc >= 4) ? argv[3] : nullptr;

    map<string, unique_ptr<qac::generator>> generator_map;

    generator_map[html_generator::get_name()] = make_unique<html_generator>();

    try {
        lexer lexer;
        vector<token> tokens = lexer.lex(input_file);
        print_token(tokens);

        parser parser;
        auto root = parser.parse(tokens);
        print_ast(root.get());

        generator_map.at(generator)
            ->generate(root.get(), input_file, output_file);
    } catch (exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}
