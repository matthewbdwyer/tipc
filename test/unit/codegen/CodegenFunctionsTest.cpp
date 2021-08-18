#include "ParserHelper.h"
#include "InternalError.h"
#include "AST.h"
#include "ASTNodeHelpers.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("CodegenFunction: ASTDeclNode throws InternalError on codegen", "[CodegenFunctions]") {
  ASTDeclNode node("foo");
  REQUIRE_THROWS_AS(node.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTAssignsStmt throws InternalError on LHS codegen nullptr", "[CodegenFunctions]") {
  ASTAssignStmt assignStmt(
      std::make_unique<nullcodegen::MockASTExpr>(),
      std::make_unique<ASTInputExpr>()
  );
  REQUIRE_THROWS_AS(assignStmt.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTAssignsStmt throws InternalError on RHS codegen nullptr", "[CodegenFunctions]") {
  ASTAssignStmt assignStmt(
      std::make_unique<ASTInputExpr>(),
      std::make_unique<nullcodegen::MockASTExpr>()
  );
  REQUIRE_THROWS_AS(assignStmt.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTIfStmt throws InternalError on COND codegen nullptr", "[CodegenFunctions]") {
  ASTIfStmt ifStmt(
      std::make_unique<nullcodegen::MockASTExpr>(),
      std::make_unique<ASTReturnStmt>(std::make_unique<ASTNumberExpr>(42)),
      std::make_unique<ASTReturnStmt>(std::make_unique<ASTNumberExpr>(42))
  );
  REQUIRE_THROWS_AS(ifStmt.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTBinaryExpr throws InternalError on LHS codegen nullptr", "[CodegenFunctions]") {
  ASTBinaryExpr binaryExpr(
      "+",
      std::make_unique<nullcodegen::MockASTExpr>(),
      std::make_unique<ASTInputExpr>()
  );
  REQUIRE_THROWS_AS(binaryExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTBinaryExpr throws InternalError on RHS codegen nullptr", "[CodegenFunctions]") {
  ASTBinaryExpr binaryExpr(
      "+",
      std::make_unique<ASTInputExpr>(),
      std::make_unique<nullcodegen::MockASTExpr>()
  );
  REQUIRE_THROWS_AS(binaryExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTBinaryExpr throws InternalError on bad OP", "[CodegenFunctions]") {
  ASTBinaryExpr binaryExpr(
      "ADDITION",
      std::make_unique<ASTInputExpr>(),
      std::make_unique<ASTInputExpr>()
  );
  REQUIRE_THROWS_AS(binaryExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTOutputStmt throws InternalError on ARG codegen nullptr", "[CodegenFunctions]") {
  ASTOutputStmt outputStmt(std::make_unique<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(outputStmt.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTErrorStmt throws InternalError on ARG codegen nullptr", "[CodegenFunctions]") {
  ASTErrorStmt errorStmt(std::make_unique<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(errorStmt.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTVariableExpr throws InternalError on unknown NAME", "[CodegenFunctions]") {
  ASTVariableExpr variableExpr("foobar");
  REQUIRE_THROWS_AS(variableExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTAllocExpr throws InternalError on INIT codegen nullptr", "[CodegenFunctions]") {
  ASTAllocExpr allocExpr(std::make_unique<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(allocExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTRefExpr throws InternalError on VAR codegen nullptr", "[CodegenFunctions]") {
  ASTRefExpr refExpr(std::make_unique<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(refExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTDeRefExpr throws InternalError on VAR codegen nullptr", "[CodegenFunctions]") {
  ASTDeRefExpr deRefExpr(std::make_unique<nullcodegen::MockASTExpr>());
  REQUIRE_THROWS_AS(deRefExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTAccessExpr throws InternalError on nonexistent field", "[CodegenFunctions]") {
  ASTAccessExpr accessExpr(
      std::make_unique<nullcodegen::MockASTExpr>(),
      "foobar"
  );
  REQUIRE_THROWS_AS(accessExpr.codegen(), InternalError);
}

TEST_CASE("CodegenFunction: ASTFunAppExpr throws InternalError on FUN codegen nullptr", "[CodegenFunctions]") {
  std::vector<std::unique_ptr<ASTExpr>> actuals;
  ASTFunAppExpr funAppExpr(
      std::make_unique<nullcodegen::MockASTExpr>(),
      std::move(actuals)
  );
  REQUIRE_THROWS_AS(funAppExpr.codegen(), InternalError);
}
