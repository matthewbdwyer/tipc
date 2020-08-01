#include "catch.hpp"
#include "ASTHelper.h"
#include "CheckAssignable.h"

#include <iostream>

TEST_CASE("Check Assignable: variable lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(varlhs() { var x; x = 1; return 0; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto ok = CheckAssignable::check(ast.get(), outputStream);
    REQUIRE(ok);
}

TEST_CASE("Check Assignable: pointer lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(ptrlhs() { var x; *x = 1; return 0; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto ok = CheckAssignable::check(ast.get(), outputStream);
    REQUIRE(ok);
}

TEST_CASE("Check Assignable: field lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(fieldlhs() { var x; x = {f:0, g:1}; x.f = 1; return 0; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto ok = CheckAssignable::check(ast.get(), outputStream);
    REQUIRE(ok);
}

TEST_CASE("Check Assignable: complex field lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(recordlhs() { var x; {f:0, g:1}.f = x; return 0; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto ok = CheckAssignable::check(ast.get(), outputStream);
    REQUIRE(ok);
}

TEST_CASE("Check Assignable: complex pointer lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(foo(x) { return &x; } ptrlhs() { var x; *foo(&x) = 1; return 0; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto ok = CheckAssignable::check(ast.get(), outputStream);
    REQUIRE(ok);
}

/************** the following are expected to fail the check ************/

TEST_CASE("Check Assignable: constant lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(constlhs() { var x; 7 = x; return 0; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto ok = CheckAssignable::check(ast.get(), outputStream);
    REQUIRE(!ok);
}

TEST_CASE("Check Assignable: binary lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(binlhs() { var x; x+1 = x; return 0; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto ok = CheckAssignable::check(ast.get(), outputStream);
    REQUIRE(!ok);
}

TEST_CASE("Check Assignable: function lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(foo() { return 0; } funlhs() { var x; foo() = x; return 0; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto ok = CheckAssignable::check(ast.get(), outputStream);
    REQUIRE(!ok);
}

TEST_CASE("Check Assignable: alloc lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(alloclhs() { var x; alloc 1 = x; return 0; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto ok = CheckAssignable::check(ast.get(), outputStream);
    REQUIRE(!ok);
}

TEST_CASE("Check Assignable: record lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(recordlhs() { var x; {f:0, g:1} = x; return 0; })";

    std::stringstream outputStream;
    auto ast = ASTHelper::build_ast(stream);

    auto ok = CheckAssignable::check(ast.get(), outputStream);
    REQUIRE(!ok);
}
