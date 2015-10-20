#include <glog/logging.h>

#include <iostream>
#include <fstream>
#include <map>
#include <set>

#include "qac/generator/html-generator.h"
#include "qac/lexer/lexer.h"
#include "qac/parser/parser.h"

using namespace qac;
using namespace std;

void print_ast(const node *node, int level = 0, bool last = false,
               std::set<int> last_set = {}) {
    static const string pipe = "│";
    static const string mux = "├";
    static const string lmux = "└";
    static const char nl = '\n';

    if (level) {
        for (int i = 0; i < level - 1; ++i) {
            bool in_last_set = last_set.count(i);
            cout << (in_last_set ? " " : pipe) << " ";
        }
        cout << (last ? lmux : mux) << "─";
    }

    const auto &children = node->children();
    int nr_children = children.size();
    cout << (nr_children ? "┬" : "") << to_string(node->type()) << nl;

    if (nr_children) {
        for (int i = 0; i < nr_children - 1; ++i) {
            print_ast(children[i].get(), level + 1, false, last_set);
        }
        last_set.insert(level);
        print_ast(children[nr_children - 1].get(), level + 1, true, last_set);
    }
}

int main(int argc, const char *argv[]) {
    google::InitGoogleLogging(argv[0]);

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
        std::ifstream input(input_file);
        if (!input.is_open()) {
            throw runtime_error("Couldn't open '" + string(input_file) + "'");
        }

        lexer lexer;
        vector<token> tokens = lexer.lex(input);

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
