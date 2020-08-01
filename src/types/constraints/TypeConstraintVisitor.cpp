#include "TypeConstraintVisitor.h"
#include "TipVar.h"
#include "TipFunction.h"
#include "TipAlpha.h"
#include "TipRef.h"
#include "TipRecord.h"
#include "TipInt.h"

/*
 * Utility function that creates type variables and uses declaration nodes
 * as a canonical representative for program variables.  There are two case
 * that need to be checked: if the variable is local to a function or if
 * it is a function value.
 */
std::shared_ptr<TipType> TypeConstraintVisitor::astToVar(ASTNode * n) {
  if (auto ve = dynamic_cast<ASTVariableExpr*>(n)) {
    ASTDeclNode * canonical;
    if (canonical = symbolTable.getLocal(ve->getName(), scope.top())) {
      return std::make_shared<TipVar>(canonical);
    } else if (canonical = symbolTable.getFunction(ve->getName())) {
      return std::make_shared<TipVar>(canonical);
    } 
  } 

  return std::make_shared<TipVar>(n);
}

bool TypeConstraintVisitor::visit(ASTFunction * element) {
    scope.push(element->getDecl());
    return true;
}

/*
 * Type rules for "main(X1, ..., Xn) { ... return E; }":
 *   [[X1]] = [[Xn]] = [[E]] = int
 * To express this we will equate all type variables to int.
 *
 * Type rules for "X(X1, ..., Xn) { ... return E; }":
 *   [[X]] = ([[X1]], ..., [[Xn]]) -> [[E]]
 */
void TypeConstraintVisitor::endVisit(ASTFunction * element) {
  if (element->getName() == "main") {
    std::vector<std::shared_ptr<TipType>> formals;
    for(auto f : element->getFormals()) {
      formals.push_back(astToVar(f));
      // all formals are int
      process(astToVar(f), std::make_shared<TipInt>());
    }

    // Return is the last statement and must be int
    auto ret = dynamic_cast<ASTReturnStmt*>(element->getStmts().back());
    process(astToVar(ret->getArg()), std::make_shared<TipInt>());

    process(astToVar(element->getDecl()),
            std::make_shared<TipFunction>(formals, astToVar(ret->getArg())));
  } else {
    std::vector<std::shared_ptr<TipType>> formals;
    for(auto f : element->getFormals()) {
      formals.push_back(astToVar(f));
    }

    // Return is the last statement 
    auto ret = dynamic_cast<ASTReturnStmt*>(element->getStmts().back());

    process(astToVar(element->getDecl()),
            std::make_shared<TipFunction>(formals, astToVar(ret->getArg())));
  }
}

/*
 * Type rules for "I":  
 *   [[I]] = int
 */
void TypeConstraintVisitor::endVisit(ASTNumberExpr * element) {
  process(astToVar(element), std::make_shared<TipInt>());
}

/*
 * Type rules for "E1 op E2":
 *   [[E1]] = [[E2]]
 *   [[E1 op E2]] = int
 * and if "op" is not equality or disequality
 *   [[E1]] = [[E1 op E2]]
 * which by transitivity also equates
 *   [[E2]] = [[E1 op E2]]
 */
void TypeConstraintVisitor::endVisit(ASTBinaryExpr  * element) {
  auto op = element->getOp();

  // operands have the same type
  process(astToVar(element->getLeft()), astToVar(element->getRight()));

  // result type is integer
  process(astToVar(element), std::make_shared<TipInt>());

  if (op != "==" && op != "!=") {
    // operand and result have same type
    process(astToVar(element->getLeft()), astToVar(element));
  }
}

/*
 * Type rules for "input":
 *  [[input]] = int
 */
void TypeConstraintVisitor::endVisit(ASTInputExpr * element) {
  process(astToVar(element), std::make_shared<TipInt>());
}

/*
 * Type Rules for "E(E1, ..., En)":
 *  [[E]] = ([[E1]], ..., [[En]]) -> [[E(E1, ..., En)]]
 */
void TypeConstraintVisitor::endVisit(ASTFunAppExpr  * element) {
  std::vector<std::shared_ptr<TipType>> actuals;
  for(auto a : element->getActuals()) {
    actuals.push_back(astToVar(a));
  }
  process(astToVar(element->getFunction()),
          std::make_shared<TipFunction>(actuals, astToVar(element)));
}

/*
 * Type Rules for "alloc E":
 *   [[alloc E]] = &[[E]]
 */
void TypeConstraintVisitor::endVisit(ASTAllocExpr * element) {
  process(astToVar(element), 
          std::make_shared<TipRef>(astToVar(element->getInitializer())));
}

/*
 * Type Rules for "&X":
 *   [[&X]] = &[[X]]
 */
void TypeConstraintVisitor::endVisit(ASTRefExpr * element) {
  process(astToVar(element), 
          std::make_shared<TipRef>(astToVar(element->getVar())));
}

/*
 * Type Rules for "*E":
 *   [[E]] = &[[*E]]
 */
void TypeConstraintVisitor::endVisit(ASTDeRefExpr * element) {
  process(astToVar(element->getPtr()), 
          std::make_shared<TipRef>(astToVar(element)));
}

/*
 * Type Rules for "null":
 *   [[null]] = & \alpha
 */
void TypeConstraintVisitor::endVisit(ASTNullExpr * element) {
  process(astToVar(element), 
          std::make_shared<TipRef>(std::make_shared<TipAlpha>("null")));
}

/* 
 * Type rules for "E1 = E":
 *   [[E1]] = [[E2]]
 *
 * Type rules for "*E1 = E2":
 *   [[E1]] = &[[E2]]
 * 
 * Note that these are slightly more general than the rules in the SPA book.
 * The first allows for record expressions on the left hand side and the second
 * allows for more complex assignments, e.g., "**p = &x"
 */
void TypeConstraintVisitor::endVisit(ASTAssignStmt  * element) {
  // If this is an assignment through a pointer, use the second rule above
  if (auto lptr = dynamic_cast<ASTDeRefExpr*>(element->getLHS())) {
    process(astToVar(lptr->getPtr()), 
            std::make_shared<TipRef>(astToVar(element->getRHS())));
  } else {
    process(astToVar(element->getLHS()), astToVar(element->getRHS()));
  }
}

/* 
 * Type rules for "while (E) S":
 *   [[E]] = int
 */
void TypeConstraintVisitor::endVisit(ASTWhileStmt * element) {
  process(astToVar(element->getCondition()), std::make_shared<TipInt>());
}

/* 
 * Type rules for "if (E) S1 else S2":
 *   [[E]] = int
 */
void TypeConstraintVisitor::endVisit(ASTIfStmt * element) {
  process(astToVar(element->getCondition()), std::make_shared<TipInt>());
}

/* 
 * Type rules for "output E":
 *   [[E]] = int
 */
void TypeConstraintVisitor::endVisit(ASTOutputStmt * element) {
  process(astToVar(element->getArg()), std::make_shared<TipInt>());
}

/*
 * Type rule for "{ X1:E1, ..., Xn:En }":
 *   [[{ X1:E1, ..., Xn:En }]] = { f1:v1, ..., fn:vn }
 * where fi is the ith field in the program's uber record
 * and vi = [[Ei]] if fi = Xi and \alpha otherwise
 */
void TypeConstraintVisitor::endVisit(ASTRecordExpr * element) {
  auto allFields = symbolTable.getFields();
  std::vector<std::shared_ptr<TipType>> fieldTypes;
  for (auto f : allFields) {
    bool matched = false;
    for (auto fe : element->getFields()) {
      if (f == fe->getField()) {
        fieldTypes.push_back(astToVar(fe->getInitializer()));
        matched = true;
        break;
      }
    }
    if (matched) continue;

    // TBD : Do we need to generate uniquely named alphas?
    fieldTypes.push_back(std::make_shared<TipAlpha>(f));
  } 
  process(astToVar(element), std::make_shared<TipRecord>(fieldTypes, allFields));
}

/*
 * Type rule for "E.X":
 *   [[E]] = { f1:v1, ..., fn:vn }
 * where fi is the ith field in the program's uber record
 * and vi = [[E.X]] if fi = X and \alpha otherwise
 */
void TypeConstraintVisitor::endVisit(ASTAccessExpr * element) {
  auto allFields = symbolTable.getFields();
  std::vector<std::shared_ptr<TipType>> fieldTypes;
  for (auto f : allFields) {
    if (f == element->getField()) {
      fieldTypes.push_back(astToVar(element));
    } else {
      fieldTypes.push_back(std::make_shared<TipAlpha>(f));
    }
  } 
  process(astToVar(element->getRecord()), 
          std::make_shared<TipRecord>(fieldTypes, allFields));
}

/* 
 * Type rules for "error E":
 *   [[E]] = int
 */
void TypeConstraintVisitor::endVisit(ASTErrorStmt * element) {
  process(astToVar(element->getArg()), std::make_shared<TipInt>());
}

