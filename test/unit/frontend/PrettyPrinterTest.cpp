#include "ASTHelper.h"
#include "GeneralHelper.h"
#include "PrettyPrinter.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

TEST_CASE("PrettyPrinter: Test default constructor", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(
      short() {
        var x, y, z;
        x = input;
        y = alloc x;
        *y = x;
        z = *y;
        z = null;
        return z;
      }
    )";

    std::stringstream devnull;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), devnull, ' ', 4);
    REQUIRE(true);
}

TEST_CASE("PrettyPrinter: Test indentation", "[PrettyPrinter]") {
  std::stringstream stream;
  stream << R"(
      foo() {
        var x;
        if(1) {
          x = 5;
        }
        else {
          x = 10;
        }
        while(1) { z = 10; }
        {
          y = 42;
        }
        return 5;
      }
    )";

  auto ast = ASTHelper::build_ast(stream);

  std::stringstream outputStream;
  PrettyPrinter prettyPrinter(outputStream, '#', 2);
  ast->accept(&prettyPrinter);
  std::vector<std::string> tokens = GeneralHelper::tokenize(outputStream.str(), '\n');

  auto token00 = tokens.at(0);
  auto token01 = tokens.at(1);
  auto token02 = tokens.at(2);
  auto token03 = tokens.at(3);
  auto token04 = tokens.at(4);
  auto token05 = tokens.at(5);
  auto token06 = tokens.at(6);
  auto token07 = tokens.at(7);
  auto token08 = tokens.at(8);
  auto token09 = tokens.at(9);
  auto token10 = tokens.at(10);
  auto token11 = tokens.at(11);
  auto token12 = tokens.at(12);
  auto token13 = tokens.at(13);
  auto token14 = tokens.at(14);
  auto token15 = tokens.at(15);
  auto token16 = tokens.at(16);
  auto token17 = tokens.at(17);
  auto token18 = tokens.at(18);
  auto token19 = tokens.at(19);

  REQUIRE(0 == std::count(token00.begin(), token00.end(), '#'));
  REQUIRE(0 == std::count(token01.begin(), token01.end(), '#'));
  REQUIRE(2 == std::count(token02.begin(), token02.end(), '#'));
  REQUIRE(2 == std::count(token03.begin(), token03.end(), '#'));
  REQUIRE(4 == std::count(token04.begin(), token04.end(), '#'));
  REQUIRE(6 == std::count(token05.begin(), token05.end(), '#'));
  REQUIRE(4 == std::count(token06.begin(), token06.end(), '#'));
  REQUIRE(2 == std::count(token07.begin(), token07.end(), '#'));
  REQUIRE(4 == std::count(token08.begin(), token08.end(), '#'));
  REQUIRE(6 == std::count(token09.begin(), token09.end(), '#'));
  REQUIRE(4 == std::count(token10.begin(), token10.end(), '#'));
  REQUIRE(2 == std::count(token11.begin(), token11.end(), '#'));
  REQUIRE(4 == std::count(token12.begin(), token12.end(), '#'));
  REQUIRE(6 == std::count(token13.begin(), token13.end(), '#'));
  REQUIRE(4 == std::count(token14.begin(), token14.end(), '#'));
  REQUIRE(2 == std::count(token15.begin(), token15.end(), '#'));
  REQUIRE(4 == std::count(token16.begin(), token16.end(), '#'));
  REQUIRE(2 == std::count(token17.begin(), token17.end(), '#'));
  REQUIRE(2 == std::count(token18.begin(), token18.end(), '#'));
  REQUIRE(0 == std::count(token19.begin(), token19.end(), '#'));
}

TEST_CASE("PrettyPrinter: Test comment removal", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(// comment
prog() { var x, y, z; output x+y; return z; })";

    std::string expected = R"(prog() 
{
  var x, y, z;
  output (x + y);
  return z;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test embedded comment removal", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog() { var x, /* comment */ y, z; output x+y; return z; })";

    std::string expected = R"(prog() 
{
  var x, y, z;
  output (x + y);
  return z;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test if print", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog() { var x; if (x) output 0; else output 1; return 0; })";

    std::string expected = R"(prog() 
{
  var x;
  if (x) 
    output 0;
  else
    output 1;
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test nested if print", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog() { var x, y; if (x) if (y) output 0; else output 1; else output 2; return 0; })";

    std::string expected = R"(prog() 
{
  var x, y;
  if (x) 
    if (y) 
      output 0;
    else
      output 1;
  else
    output 2;
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test paren expr", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog() { var x, y; x = y * 3 + 4 - y; return 0; })";

    std::string expected = R"(prog() 
{
  var x, y;
  x = (((y * 3) + 4) - y);
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test while spacing", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog(){var x,y;while(y>0){x=x+y;y=y-1;}return x;})";

    std::string expected = R"(prog() 
{
  var x, y;
  while ((y > 0)) 
    {
      x = (x + y);
      y = (y - 1);
    }
  return x;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}


TEST_CASE("PrettyPrinter: Test funs and calls", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(fun(a){return a+1;}main() {output fun(9); return fun(1) + fun(2);})";

    std::string expected = R"(fun(a) 
{
  return (a + 1);
}

main()        
{
  output fun(9);
  return (fun(1) + fun(2));
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}
