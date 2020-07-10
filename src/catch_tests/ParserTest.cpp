#include "catch.hpp"
#include "ASTHelper.h"

#include <iostream>

TEST_CASE("Parser: control structures", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      short() {
        var x, y, z;
        if (x>0) {
          while (y>z) {
            y = y + 1;
          }
        } else {
          z = z + 1;
        }
        return z;
      }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: missing semi-colon", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      short() { var x; if (x>0) x = x + 1 return 0; }
    )";

    REQUIRE_FALSE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: missing paren", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      short() { var x; if x>0 x = x + 1; return 0; }
    )";

    REQUIRE_FALSE(ASTHelper::is_parsable(stream));
}


