#include "SemanticError.h"
#include "LocalNameCollector.h"
#include "ASTNodeHelpers.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>


TEST_CASE("LocalNameCollector: Test identifier conflicts with function declaration", "[LocalNameCollector]") {
  // Seed the function map and visit its declaration.
  std::map<std::string, ASTDeclNode*> fmap;
  ASTDeclNode mockFunctionNameDecl("foobar");
  fmap.insert({"foobar", &mockFunctionNameDecl});
  LocalNameCollector collector(fmap);
  mockFunctionNameDecl.accept(&collector);

  // Now visit a declaration of the same name.
  ASTDeclNode mockShadow("foobar");
  REQUIRE_THROWS_AS(mockShadow.accept(&collector), SemanticError);
}
