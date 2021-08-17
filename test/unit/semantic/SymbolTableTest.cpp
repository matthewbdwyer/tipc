#include "ASTHelper.h"
#include "SymbolTable.h"
#include "SemanticError.h"
#include "ExceptionContainsWhat.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <optional>


TEST_CASE("Symbol Table: locals", "[SymbolTable]") {
    std::stringstream stream;
    stream << R"(short() { var x, y, z; output x+y; return z; })";

    auto ast = ASTHelper::build_ast(stream);

    std::unique_ptr<SymbolTable> symbols;
    REQUIRE_NOTHROW(symbols = SymbolTable::build(ast.get()));

    std::stringstream outputStream;
    symbols->print(outputStream);
    std::string output = outputStream.str();

    std::size_t found = output.find("Functions : {short}");
    REQUIRE(found!=std::string::npos);

    found = output.find("short : {x, y, z}");
    REQUIRE(found!=std::string::npos);
}


TEST_CASE("Symbol Table: functions", "[SymbolTable]") {
    std::stringstream stream;
    stream << R"(foo() { return 0; } bar() { return 1; } baz() { return 2;})";

    auto ast = ASTHelper::build_ast(stream);

    std::unique_ptr<SymbolTable> symbols;
    REQUIRE_NOTHROW(symbols = SymbolTable::build(ast.get()));

    std::stringstream outputStream;
    symbols->print(outputStream);
    std::string output = outputStream.str();

    std::size_t found = output.find("Functions : {bar, baz, foo}");
    REQUIRE(found!=std::string::npos);

    found = output.find("foo : {}");
    REQUIRE(found!=std::string::npos);
}

TEST_CASE("Symbol Table: params", "[SymbolTable]") {
    std::stringstream stream;
    stream << R"(foo(a) { return a; } bar(a) { return a; } baz(b) { return b;})";

    auto ast = ASTHelper::build_ast(stream);

    std::unique_ptr<SymbolTable> symbols;
    REQUIRE_NOTHROW(symbols = SymbolTable::build(ast.get()));

    std::stringstream outputStream;
    symbols->print(outputStream);
    std::string output = outputStream.str();

    std::size_t found = output.find("Functions : {bar, baz, foo}");
    REQUIRE(found!=std::string::npos);

    found = output.find("foo : {a}");
    REQUIRE(found!=std::string::npos);

    found = output.find("bar : {a}");
    REQUIRE(found!=std::string::npos);

    found = output.find("baz : {b}");
    REQUIRE(found!=std::string::npos);
}

TEST_CASE("Symbol Table: locals params ", "[SymbolTable]") {
    std::stringstream stream;
    stream << R"(short(a, b, c) { var x, y, z; return z; })";

    auto ast = ASTHelper::build_ast(stream);

    std::unique_ptr<SymbolTable> symbols;
    REQUIRE_NOTHROW(symbols = SymbolTable::build(ast.get()));

    std::stringstream outputStream;
    symbols->print(outputStream);
    std::string output = outputStream.str();

    std::size_t found = output.find("Functions : {short}");
    REQUIRE(found!=std::string::npos);

    found = output.find("short : {a, b, c, x, y, z}");
    REQUIRE(found!=std::string::npos);
}

TEST_CASE("SymbolTable: fields", "[SymbolTable]") {
    std::stringstream stream;
    stream << R"(
      main(){
        var rec1, rec2;
        rec1 = {a:1, b:2, c:3};
        rec2 = {d:4, e:5, f:6};
        return 0;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);

    std::unique_ptr<SymbolTable> symbols;
    REQUIRE_NOTHROW(symbols = SymbolTable::build(ast.get()));

    std::stringstream outputStream;
    symbols->print(outputStream);
    std::string output = outputStream.str();

    std::size_t found = output.find("Fields : {a, b, c, d, e, f}");
    REQUIRE(found!=std::string::npos);
}


/******************** symbol errors *******************/

TEST_CASE("Symbol Table: local undeclared", "[SymbolTable]") {
    std::stringstream stream;
    stream << R"(short(a, b, c) { var x; return z; })";

    auto ast = ASTHelper::build_ast(stream);

    REQUIRE_THROWS_MATCHES(SymbolTable::build(ast.get()),
                           SemanticError,
                           ContainsWhat("z undeclared"));
}

TEST_CASE("Symbol Table: locals param clash ", "[SymbolTable]") {
    std::stringstream stream;
    stream << R"(short(a, b, c) { var x, b, z; return z; })";

    auto ast = ASTHelper::build_ast(stream);

    REQUIRE_THROWS_MATCHES(SymbolTable::build(ast.get()),
                           SemanticError,
                           ContainsWhat("b redeclared"));
}

TEST_CASE("Symbol Table: locals clash ", "[SymbolTable]") {
    std::stringstream stream;
    stream << R"(short(a, b, c) { var x, x, z; return z; })";

    auto ast = ASTHelper::build_ast(stream);

    REQUIRE_THROWS_MATCHES(SymbolTable::build(ast.get()),
                           SemanticError,
                           ContainsWhat("x redeclared"));
}

TEST_CASE("Symbol Table: params clash ", "[SymbolTable]") {
    std::stringstream stream;
    stream << R"(short(a, b, a) { var x, y, z; return z; })";

    auto ast = ASTHelper::build_ast(stream);

    REQUIRE_THROWS_MATCHES(SymbolTable::build(ast.get()),
                           SemanticError,
                           ContainsWhat("a redeclared"));
}

TEST_CASE("Symbol Table: functions clash ", "[SymbolTable]") {
    std::stringstream stream;
    stream << R"(foo() { return 0; } bar() { return 1; } foo() { return 2;})";

    auto ast = ASTHelper::build_ast(stream);

    REQUIRE_THROWS_MATCHES(SymbolTable::build(ast.get()),
                           SemanticError,
                           ContainsWhat("foo already declared"));
}

TEST_CASE("Symbol Table: Unknown Function", "[SymbolTable]") {
  std::stringstream stream;
  stream << R"(short() { var x, y, z; output x+y; return z; })";
  auto ast = ASTHelper::build_ast(stream);
  std::unique_ptr<SymbolTable> symbols = SymbolTable::build(ast.get());
  REQUIRE(nullptr == symbols->getFunction("foo"));
}
