#include "LocalNameCollector.h"
#include "ASTNodeHelpers.h"
#include "SemanticError.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>

TEST_CASE(
    "LocalNameCollector: Test identifier conflicts with function declaration",
    "[LocalNameCollector]") {
  // Seed the function map and visit its declaration.
  std::map<std::string, std::pair<ASTDeclNode *, bool>> fmap;
  ASTDeclNode mockFunctionNameDecl("foobar");
  fmap.insert({"foobar", std::make_pair(&mockFunctionNameDecl, false)});
  LocalNameCollector collector(fmap);
  mockFunctionNameDecl.accept(&collector);

  // Now visit a declaration of the same name.
  ASTDeclNode mockShadow("foobar");
  REQUIRE_THROWS_AS(mockShadow.accept(&collector), SemanticError);
}
