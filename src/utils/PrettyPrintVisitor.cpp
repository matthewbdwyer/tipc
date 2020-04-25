#include "PrettyPrintVisitor.h"
#include <iostream>

void PrettyPrintVisitor::visit(TIPtree::Program * element) {
  for (auto &fn : element->FUNCTIONS) {
    fn->accept(this);
    os << "\n";
  }
  os.flush();
}

void PrettyPrintVisitor::visit(TIPtree::Function * element) {
  os << element->NAME << "(";

  bool skip = true;
  for(auto &formal : element->FORMALS) {
    if (skip) {
      os << formal;
      skip = false;
      continue;
    }

    os << ", " << formal;
  }
  os << ")";

  safePrintLineNumber(element->LINE);
  os << "\n";

  indentLevel++;
  os << indent() << "{\n";
  indentLevel++;

  for (auto &decl : element->DECLS) {
    os << indent();
    decl->accept(this);
    os << "\n";
  }

  for (auto &stmt : element->BODY) {
    os << indent();
    stmt->accept(this);
    os << "\n";
  }

  indentLevel--;
  os << indent() << "}\n";
  indentLevel--;
}

void PrettyPrintVisitor::visit(TIPtree::NumberExpr * element) {
  os << element->VAL;
}

void PrettyPrintVisitor::visit(TIPtree::VariableExpr * element) {
  os << element->NAME;
}

void PrettyPrintVisitor::visit(TIPtree::BinaryExpr * element) {
  os << "(";
  element->LHS->accept(this);
  os << " " << element->OP;
  element->RHS->accept(this);
  os << ")";
}

void PrettyPrintVisitor::visit(TIPtree::InputExpr * element) {
  os << "input";
}

void PrettyPrintVisitor::visit(TIPtree::FunAppExpr * element) {
  element->FUN->accept(this);
  os << "(";

  bool skip = true;
  for (auto  &arg : element->ACTUALS) {
    if (skip) {
      arg->accept(this);
      skip = false;
      continue;
    }

    os << ", ";
    arg->accept(this);
  }
  os << ")";
}

void PrettyPrintVisitor::visit(TIPtree::AllocExpr * element) {
  os << "alloc ";
  element->ARG->accept(this);
}

void PrettyPrintVisitor::visit(TIPtree::RefExpr * element) {
  os << "&" << element->NAME;
}

void PrettyPrintVisitor::visit(TIPtree::DeRefExpr * element) {
  os << "*";
  element->ARG->accept(this);
}

void PrettyPrintVisitor::visit(TIPtree::NullExpr * element) {
  os << "null";
}

void PrettyPrintVisitor::visit(TIPtree::FieldExpr * element) {
  os << element->FIELD << ":";
  element->INIT->accept(this);
}

void PrettyPrintVisitor::visit(TIPtree::RecordExpr * element) {
  os << "{";
  bool skip = true;
  for (auto  &field : element->FIELDS) {
    if (skip) {
      field->accept(this);
      skip = false;
      continue;
    }

    os << ", ";
    field->accept(this);
  }
  os << "}";
}

void PrettyPrintVisitor::visit(TIPtree::AccessExpr * element) {
  element->RECORD->accept(this);
  os << "." << element->FIELD;
}

void PrettyPrintVisitor::visit(TIPtree::DeclStmt * element) {
  bool skip = true;
  for (auto &id : element->VARS) {
    if (skip) {
      os << id;
      skip = false;
      continue;
    }

    os << ", " << id;
  }

  os << ";";
  safePrintLineNumber(element->LINE);
}

void PrettyPrintVisitor::visit(TIPtree::AssignStmt * element) {
  element->LHS->accept(this);
  os << " = ";
  element->RHS->accept(this);
  os << ";";
}

void PrettyPrintVisitor::visit(TIPtree::BlockStmt * element) {
  os << "{\n";
  indentLevel++;
  for (auto &s : element->STMTS) {
    os << indent();
    s->accept(this);
    os << "\n";
  }

  indentLevel--;
  os << indent() << "}";
}

void PrettyPrintVisitor::visit(TIPtree::WhileStmt * element) {
  os << "while (";
  element->COND->accept(this);
  os << ") \n";
  indentLevel++;
  os << indent();
  element->BODY->accept(this);
  indentLevel--;
}

void PrettyPrintVisitor::visit(TIPtree::IfStmt * element) {
  os << "if (";
  element->COND->accept(this);
  os << ") \n";
  indentLevel++;

  os << indent();
  element->THEN->accept(this);

  if (element->ELSE != nullptr) {
    indentLevel--;
    os << "\n" << indent() << "else\n";
    indentLevel++;
    os << indent();
    element->ELSE->accept(this);
  }
  indentLevel--;
}

void PrettyPrintVisitor::visit(TIPtree::OutputStmt * element) {
  os << "output ";
  element->ARG->accept(this);
  os << ";";
}

void PrettyPrintVisitor::visit(TIPtree::ErrorStmt * element) {
  os << "error ";
  element->ARG->accept(this);
  os << ";";
}

void PrettyPrintVisitor::visit(TIPtree::ReturnStmt * element) {
  os << "return ";
  element->ARG->accept(this);
  os << ";";
}

std::string PrettyPrintVisitor::indent() const {
  return std::string(indentLevel*indentSize, indentChar);
}

void PrettyPrintVisitor::safePrintLineNumber(int lineNumber) const {
  if (printLines) {
    os << " // @" << lineNumber;
  }
}
