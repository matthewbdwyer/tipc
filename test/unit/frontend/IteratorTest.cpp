#include "ASTHelper.h"
#include "PrettyPrinter.h"
#include "SyntaxTree.h"
#include "PreOrderIterator.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <Iterator.h>

TEST_CASE("Iterator: Test copy constructor", "[Iterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return 5;
      }
    )";
  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));
  SyntaxTree syntaxTree(ast);
  Iterator const begin = syntaxTree.begin("");
  auto copy = Iterator(begin);
  for(; copy != syntaxTree.end(""); copy++);
  REQUIRE(true);
}

TEST_CASE("Iterator: Test getTree", "[Iterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return 5;
      }
    )";
  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));
  SyntaxTree syntaxTree(ast);
  auto iter = syntaxTree.begin("");
  auto tree = iter.get_tree();
  REQUIRE(syntaxTree.getRoot() == tree.getRoot());
}
