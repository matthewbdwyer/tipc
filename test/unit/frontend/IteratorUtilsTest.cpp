#include "ASTHelper.h"
#include "PrettyPrinter.h"
#include "SyntaxTree.h"
#include "PreOrderIterator.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <Iterator.h>

class MockIteratorA: public IteratorImpl {
public:
  MockIteratorA(const SyntaxTree &tree) : IteratorImpl(tree) {}
  SyntaxTree operator*() override { return SyntaxTree(nullptr);}
  SyntaxTree *operator->() override { return nullptr; }
  void operator++() override { }
  bool operator==(const IteratorImpl &rhs) const override { return false; }
  bool operator!=(const IteratorImpl &rhs) const override { return false; }
  IteratorImpl *clone() override { return nullptr; }
};

TEST_CASE("IteratorUtils: Different iterator impls cannot be equal", "[IteratorUtils]") {
  SyntaxTree tree(nullptr);
  PreOrderIterator * p = new PreOrderIterator(tree, false);
  MockIteratorA a(tree);

  REQUIRE_FALSE(IteratorUtils::are_equal(p, a));
}

TEST_CASE("IteratorUtils: Iterators on different trees are not equal", "[IteratorUtils]") {
  std::stringstream streamA;
  streamA << R"(
      short() {
        var x;
        return 5;
      }
    )";
  std::shared_ptr<ASTProgram> astA = std::move(ASTHelper::build_ast(streamA));
  SyntaxTree syntaxTreeA(astA);

  std::stringstream streamB;
  streamB << R"(
      foo() {
        var f;
        return 42;
      }
    )";
  std::shared_ptr<ASTProgram> astB = std::move(ASTHelper::build_ast(streamB));
  SyntaxTree syntaxTreeB(astB);

  auto iterA = PreOrderIterator(syntaxTreeA, false);
  auto iterB = PreOrderIterator(syntaxTreeB, false);

  REQUIRE_FALSE(IteratorUtils::are_equal(&iterA, iterB));
}

TEST_CASE("IteratorUtils: Iterators of same type and pointing to same spot are equalt", "[IteratorUtils]") {
  std::stringstream streamA;
  streamA << R"(
      short() {
        var x;
        return 5;
      }
    )";
  std::shared_ptr<ASTProgram> astA = std::move(ASTHelper::build_ast(streamA));
  SyntaxTree syntaxTreeA(astA);

  auto iterA = PreOrderIterator(syntaxTreeA, false);
  auto iterB = PreOrderIterator(syntaxTreeA, false);

  REQUIRE(IteratorUtils::are_equal(&iterA, iterB));
}
