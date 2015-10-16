#include "qac/parser/parser.h"

using namespace qac;
using namespace std;

unique_ptr<node> parser::parse(const std::vector<token> &tokens) {
    unique_ptr<node> root = make_unique<node>(node_enum::ROOT);

    auto q1 = make_unique<node>(node_enum::QUESTION);
    auto a1 = make_unique<node>(node_enum::ANSWER);
    auto a11 = make_unique<node>(node_enum::ANSWER);
    auto q2 = make_unique<node>(node_enum::QUESTION);
    auto a2 = make_unique<node>(node_enum::ANSWER);

    q1->add_child(std::move(a1));
    q1->add_child(std::move(a11));
    q2->add_child(std::move(a2));

    root->add_child(std::move(q1));
    root->add_child(std::move(q2));

    //for (vector<token>::const_iterator it = cbegin(tokens); it != cend(tokens); ++it) {
        //root->add_child(parse(tokens, it));
    //}
    return root;
}

unique_ptr<node> parser::parse(const vector<token> &tokens, vector<token>::const_iterator &current) {
    return nullptr;
}
