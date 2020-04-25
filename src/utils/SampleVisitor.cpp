#include "SampleVisitor.h"
#include <iostream>

void SampleVisitor::visit(TIPtree::Program * element) {
    std::cout << "visit_program" << std::endl;
    for (auto &fn : element->FUNCTIONS) {
        fn->accept(this);
    }
}

void SampleVisitor::visit(TIPtree::Function * element) {
    std::cout << "visit_function" << std::endl;
    for (auto  &decl : element->DECLS) {
        decl->accept(this);
    }

    for (auto  &stmt : element->BODY) {
        stmt->accept(this);
    }
}

void SampleVisitor::visit(TIPtree::NumberExpr * element) {
    std::cout << "visit_numberExpr" << std::endl;
}

void SampleVisitor::visit(TIPtree::VariableExpr * element) {
    std::cout << "visit_variableExpr" << std::endl;
}

void SampleVisitor::visit(TIPtree::BinaryExpr * element) {
    std::cout << "visit_binaryExpr" << std::endl;
    element->LHS->accept(this);
    element->RHS->accept(this);
}

void SampleVisitor::visit(TIPtree::InputExpr * element) {
    std::cout << "visit_inputExpr" << std::endl;
}

void SampleVisitor::visit(TIPtree::FunAppExpr * element) {
    std::cout << "visit_funAppExpr" << std::endl;
    element->FUN->accept(this);
    for (auto  &arg : element->ACTUALS) {
        arg->accept(this);
    }
}

void SampleVisitor::visit(TIPtree::AllocExpr * element) {
    std::cout << "visit_allocExpr" << std::endl;
    element->ARG->accept(this);
}

void SampleVisitor::visit(TIPtree::RefExpr * element) {
    std::cout << "visit_refExpr" << std::endl;
}

void SampleVisitor::visit(TIPtree::DeRefExpr * element) {
    std::cout << "visit_deRefExpr" << std::endl;
    element->ARG->accept(this);
}

void SampleVisitor::visit(TIPtree::NullExpr * element) {
    std::cout << "visit_nullExpr" << std::endl;
}

void SampleVisitor::visit(TIPtree::FieldExpr * element) {
  std::cout << "visit_fieldExp" << std::endl;
  element->INIT->accept(this);
}

void SampleVisitor::visit(TIPtree::RecordExpr * element) {
  std::cout << "visit_recordEpr" << std::endl;
  for (auto  &field : element->FIELDS) {
    field->accept(this);
  }
}

void SampleVisitor::visit(TIPtree::AccessExpr * element) {
  std::cout << "visit_accessExp" << std::endl;
  element->RECORD->accept(this);
}

void SampleVisitor::visit(TIPtree::DeclStmt * element) {
    std::cout << "visit_declStmt" << std::endl;
}

void SampleVisitor::visit(TIPtree::AssignStmt * element) {
    std::cout << "visit_assignStmt" << std::endl;
    element->LHS->accept(this);
    element->RHS->accept(this);
}

void SampleVisitor::visit(TIPtree::BlockStmt * element) {
  std::cout << "visit_blockStmt" << std::endl;
  for (auto  &s : element->STMTS) {
    s->accept(this);
  }
}

void SampleVisitor::visit(TIPtree::WhileStmt * element) {
    std::cout << "visit_whileStmt" << std::endl;
    element->COND->accept(this);
    element->BODY->accept(this);
}

void SampleVisitor::visit(TIPtree::IfStmt * element) {
    std::cout << "visit_ifStmt" << std::endl;
    element->COND->accept(this);
    element->THEN->accept(this);

    if (element->ELSE != nullptr) {
        element->ELSE->accept(this);
    }
}

void SampleVisitor::visit(TIPtree::OutputStmt * element) {
    std::cout << "visit_outputStmt" << std::endl;
    element->ARG->accept(this);
}

void SampleVisitor::visit(TIPtree::ErrorStmt * element) {
    std::cout << "visit_errorStmt" << std::endl;
    element->ARG->accept(this);
}

void SampleVisitor::visit(TIPtree::ReturnStmt * element) {
  std::cout << "visit_returnStmt" << std::endl;
  element->ARG->accept(this);
}
