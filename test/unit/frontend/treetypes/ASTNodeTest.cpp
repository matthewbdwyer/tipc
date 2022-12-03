#include "ASTHelper.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <sstream>
#include "ASTVisitor.h"

class RecordPostPrint : public ASTVisitor {
public:
    std::vector<std::string> postPrintStrings;
    void record(ASTNode * element) {
       std::stringstream s; s << *element; postPrintStrings.push_back(s.str()); 
    }
    void endVisit(ASTProgram * element) { record(element); }
    void endVisit(ASTFunction * element) { record(element); }
    void endVisit(ASTNumberExpr * element) { record(element); }
    void endVisit(ASTVariableExpr * element) { record(element); }
    void endVisit(ASTBinaryExpr * element) { record(element); }
    void endVisit(ASTInputExpr * element) { record(element); }
    void endVisit(ASTFunAppExpr * element) { record(element); }
    void endVisit(ASTAllocExpr * element) { record(element); }
    void endVisit(ASTRefExpr * element) { record(element); }
    void endVisit(ASTDeRefExpr * element) { record(element); }
    void endVisit(ASTNullExpr * element) { record(element); }
    void endVisit(ASTFieldExpr * element) { record(element); }
    void endVisit(ASTRecordExpr * element) { record(element); }
    void endVisit(ASTAccessExpr * element) { record(element); }
    void endVisit(ASTDeclNode * element) { record(element); }
    void endVisit(ASTDeclStmt * element) { record(element); }
    void endVisit(ASTAssignStmt * element) { record(element); }
    void endVisit(ASTWhileStmt * element) { record(element); }
    void endVisit(ASTIfStmt * element) { record(element); }
    void endVisit(ASTOutputStmt * element) { record(element); }
    void endVisit(ASTReturnStmt * element) { record(element); }
    void endVisit(ASTErrorStmt * element) { record(element); }
    void endVisit(ASTBlockStmt * element) { record(element); }
};

// Helper function that checks for raw node pointer in list
bool contains(std::vector<std::shared_ptr<ASTNode>> nodeList, ASTNode * nodeP) {
  bool found = false;
  for (auto & sharedNodeP : nodeList) {
    if (sharedNodeP.get() == nodeP) {
      found = true;
      break;
    }
  }
  return found;
}

TEST_CASE("ASTNodeTest: ASTAssign", "[ASTNode]") {
  auto rhs = std::make_unique<ASTNumberExpr>(42);
  auto lhs = std::make_unique<ASTVariableExpr>("x");

  // Record the raw pointers for these values because rhs and lhs will not be 
  // usable after the call to the constructor below.  This is because of the
  // move semantics associated with unique pointers, i.e., after the move the
  // locals rhs and lhs are nullptrs
  auto rhsValue = rhs.get();
  auto lhsValue = lhs.get();

  auto assign = std::make_unique<ASTAssignStmt>(std::move(lhs), std::move(rhs));

  // Test Print Method
  std::stringstream nodePrintStream;
  nodePrintStream << *assign;
  REQUIRE(nodePrintStream.str() == "x = 42;");

  // Test getters 
  REQUIRE(rhsValue == assign->getRHS());
  REQUIRE(lhsValue == assign->getLHS());

  // Test getChildren
  auto children = assign->getChildren();
  REQUIRE(children.size() == 2);
  REQUIRE(contains(children, rhsValue));
  REQUIRE(contains(children, lhsValue));

  // Test accept
  RecordPostPrint visitor;
  assign->accept(&visitor);
  std::string expected[] = { "x", "42", "x = 42;" };
  for (int i=0; i < 3; i++) {
    REQUIRE(visitor.postPrintStrings[i] == expected[i]);
  }
}

