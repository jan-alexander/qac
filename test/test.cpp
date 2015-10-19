#define CATCH_CONFIG_MAIN
#include "catch.hpp"

template <>
std::string Catch::toString<qac::token>(token const& value) {
    return "token";
}
