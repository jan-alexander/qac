#include "qac/parser/parser.h"

using namespace qac;
using namespace std;

node::node(node_enum type) : type_(type) {}

unique_ptr<node> parser::parse(std::vector<token> tokens) {
    return make_unique<node>(node_enum::ROOT);
}