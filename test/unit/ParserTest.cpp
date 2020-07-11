#include "catch.hpp"
#include "ASTHelper.h"

#include <iostream>

TEST_CASE("Parser: conditionals", "[Parser]") {
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

TEST_CASE("Parser: operators", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      operators() {
        var x;
        x = y + 1;
        x = y - 1;
        x = y * 1;
        x = y / 1;
        x = -1;
        x = 1 > 0;
        x = 1 == 0;
        x = 1 != 0;
        return z;
      }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: pointers", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      operators() {
        var x, y;
        y = alloc 13;
        x = &y;
        *y = 42;
        **x = 7;
        x = null;
        return *y;
      }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: funs", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      foo(f, a) { return f(a); }
      bar(x) { return x + 1; }
      baz(y) { return foo(bar, y); }
      main(z) { return baz(z); }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: decls", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      main() { var x; var y; var z; return 0; }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: parens", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      main() { return ((1 + 2) * 3) - ((((2 - 1)))); }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: io stmts", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      main() { var x; x = input; output x; error x; output x * x; error (x * x); return x; }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: records", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      r() { var x, y; x = {f1:0, f2:13}; y = x.f1; x.f2 = y + a; return x.f2; }
      nr() { var x, y; x = {f1: {nf1:1, nf2:2}, f2:13}; y = x.f1.nf1; x.f1.nf2 = x.f2; return x.f1.nf1; }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: block stmts", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      main() { var x, y; { x = 0; { y = x + 1; } } return x + y; }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: identifiers and literals", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      main() { var __314, __; __314 = 00007; __ = 0000; return __; }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: dangling else", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      operators() { var x; if (x==0) if (x==0) x = x + 1; else x = x-1; return x; }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: input", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      operators() { var x; if (input) if (input) x = 1; else x = -1; return x; }
      outin() { output input; return 0; }
    )";

    REQUIRE(ASTHelper::is_parsable(stream));
}


/************ The following are expected to fail parsing ************/

TEST_CASE("Parser: decl after stmt", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      main() { var x; x = 0; var z; return 0; }
    )";

    REQUIRE_FALSE(ASTHelper::is_parsable(stream));
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

TEST_CASE("Parser: imbalanced blocks", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      main() { var x, y; { x = 0; y = x + 1; } } return x + y; }
    )";

    REQUIRE_FALSE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: imbalanced binary expr", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      operators() { var x; x = y + + 1; return -x; }
    )";

    REQUIRE_FALSE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: address of expr", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      operators() { var x, y; y = &(x+1); return *y; }
    )";

    REQUIRE_FALSE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: bad field delimiter", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      operators() { var x; x = {a:0, b 0}; return x.a; }
    )";

    REQUIRE_FALSE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: bad field separator", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      operators() { var x; x = {a:0 b:0}; return x.a; }
    )";

    REQUIRE_FALSE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: no expression statements", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      operators() { var x, y; x = y = 1; return x; }
    )";

    REQUIRE_FALSE(ASTHelper::is_parsable(stream));
}

TEST_CASE("Parser: missing comparison", "[Parser]") {
    std::stringstream stream;
    stream << R"(
      operators() { var x; if (x <= 0) x = x + 1; return x; }
    )";

    REQUIRE_FALSE(ASTHelper::is_parsable(stream));
}


