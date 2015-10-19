#include "catch.hpp"
#include "qac/lexer/lexer.h"

#include <sstream>
#include <vector>

using namespace qac;

TEST_CASE("lexer test", "[lexer]") {
    SECTION("hello world lexer text") {
        std::stringstream ss;

        ss << "Q: Hello\n"
           << "A: World\n";

        std::vector<token> expected = {
            token(token_enum::QUESTION, "Q:", 1),
            token(token_enum::WORD, "Hello", 1),
            token(token_enum::NEW_LINE, "", 1),

            token(token_enum::ANSWER, "A:", 2),
            token(token_enum::WORD, "World", 2),
            token(token_enum::NEW_LINE, "", 2),
        };

        lexer l;
        std::vector<token> actual = l.lex(ss);

        REQUIRE(expected.size() == actual.size());
        REQUIRE(expected == actual);
    }
}
