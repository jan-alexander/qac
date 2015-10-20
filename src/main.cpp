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

DEFINE_bool(listgenerators, false, "List available generators.");
DEFINE_bool(printtree, false, "Print parse tree.");
DEFINE_string(generator, "html", "Used generator.");
DEFINE_string(output, "", "File to write output to.");

void print_ast(const node *node, int level = 0, bool last = false,
               std::set<int> last_set = {}) {
    static const string pipe = "│";
    static const string mux = "├";
    static const string lmux = "└";

    if (level) {
        for (int i = 0; i < level - 1; ++i) {
            bool in_last_set = last_set.count(i);
            cout << (in_last_set ? " " : pipe) << " ";
        }
        cout << (last ? lmux : mux) << "─";
    }

    const auto &children = node->children();
    int nr_children = children.size();
    cout << (nr_children ? "┬" : "") << to_string(node->type()) << "\n";

    if (nr_children) {
        for (int i = 0; i < nr_children - 1; ++i) {
            print_ast(children[i].get(), level + 1, false, last_set);
        }
        last_set.insert(level);
        print_ast(children[nr_children - 1].get(), level + 1, true, last_set);
    }
}

void add_generator(map<string, unique_ptr<generator>> &generator_map,
                   unique_ptr<generator> generator) {
    generator_map[generator->get_name()] = std::move(generator);
}

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage("[flags] <qa-file>");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    map<string, unique_ptr<qac::generator>> generator_map;
    add_generator(generator_map, make_unique<html_generator>());

    if (FLAGS_listgenerators) {
        for (const auto &it : generator_map) {
            cout << it.second->get_name() << "\t\t"
                 << it.second->get_description() << endl;
        }
        return 1;
    }

    if (argc < 2) {
        gflags::ShowUsageWithFlagsRestrict(argv[0], "main.cpp");
        return 1;
    }

    const char *input_file = argv[1];

    try {
        std::ifstream input(input_file);
        if (!input.is_open()) {
            throw runtime_error("Couldn't open '" + string(input_file) + "'");
        }

        bool use_stdout = FLAGS_output.empty();
        std::ofstream output;
        if (!use_stdout) {
            output.open(FLAGS_output);
            if (!output.is_open()) {
                throw runtime_error("Couldn't open '" + FLAGS_output + "'");
            }
        }

        lexer lexer;
        vector<token> tokens = lexer.lex(input);

        parser parser;
        auto root = parser.parse(tokens);

        if (FLAGS_printtree) {
            print_ast(root.get());
        }

        generator_map.at(FLAGS_generator)
            ->generate(root.get(), use_stdout ? cout : output);
    } catch (exception &e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}
