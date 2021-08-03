#include "ASTHelper.h"
#include "PrettyPrinter.h"
#include "SyntaxTree.h"

#include <catch2/catch.hpp>

#include <iostream>
#include <Iterator.h>

TEST_CASE("PreOrderIterator: Test Traversal", "[PreOrderIterator]") {
  std::stringstream stream;
  stream << R"(
      short() {
        var x;
        return 5;
      }
    )";

  std::shared_ptr<ASTProgram> ast = std::move(ASTHelper::build_ast(stream));

  std::vector<std::string> expected_node_order = {
      "",
      "short() {...}",
      "short",
      "var x;",
      "x",
      "return 5;",
      "5",
  };

  SyntaxTree syntaxTree(ast);
  int i = 0;
  for(auto iter = syntaxTree.begin(""); iter != syntaxTree.end(""); ++iter) {
    std::stringstream actual_node;
    actual_node << *iter->getRoot();
    REQUIRE(expected_node_order.at(i++) == actual_node.str());
  }
}
