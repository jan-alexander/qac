#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "qac/lexer/lexer.h"

#include <sstream>

template <>
std::string Catch::toString<qac::token>(qac::token const& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
