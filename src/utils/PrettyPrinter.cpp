#include "PrettyPrinter.h"
#include <iostream>

void PrettyPrinter::print(std::unique_ptr<AST::Program> p,
                          std::ostream &s,
                          char c, int n, bool wlines) {
   PrettyPrinter visitor(s, c, n, wlines);
   p->accept(&visitor);
}

void PrettyPrinter::visit(AST::Program * element) {
  for (auto &fn : element->FUNCTIONS) {
    fn->accept(this);
    os << "\n";
  }
  os.flush();
}

void PrettyPrinter::visit(AST::Function * element) {
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

void PrettyPrinter::visit(AST::NumberExpr * element) {
  os << element->VAL;
}

void PrettyPrinter::visit(AST::VariableExpr * element) {
  os << element->NAME;
}

void PrettyPrinter::visit(AST::BinaryExpr * element) {
  os << "(";
  element->LHS->accept(this);
  os << " " << element->OP;
  element->RHS->accept(this);
  os << ")";
}

void PrettyPrinter::visit(AST::InputExpr * element) {
  os << "input";
}

void PrettyPrinter::visit(AST::FunAppExpr * element) {
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

void PrettyPrinter::visit(AST::AllocExpr * element) {
  os << "alloc ";
  element->ARG->accept(this);
}

void PrettyPrinter::visit(AST::RefExpr * element) {
  os << "&" << element->NAME;
}

void PrettyPrinter::visit(AST::DeRefExpr * element) {
  os << "*";
  element->ARG->accept(this);
}

void PrettyPrinter::visit(AST::NullExpr * element) {
  os << "null";
}

void PrettyPrinter::visit(AST::FieldExpr * element) {
  os << element->FIELD << ":";
  element->INIT->accept(this);
}

void PrettyPrinter::visit(AST::RecordExpr * element) {
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

void PrettyPrinter::visit(AST::AccessExpr * element) {
  element->RECORD->accept(this);
  os << "." << element->FIELD;
}

void PrettyPrinter::visit(AST::DeclStmt * element) {
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

void PrettyPrinter::visit(AST::AssignStmt * element) {
  element->LHS->accept(this);
  os << " = ";
  element->RHS->accept(this);
  os << ";";
}

void PrettyPrinter::visit(AST::BlockStmt * element) {
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

void PrettyPrinter::visit(AST::WhileStmt * element) {
  os << "while (";
  element->COND->accept(this);
  os << ") \n";
  indentLevel++;
  os << indent();
  element->BODY->accept(this);
  indentLevel--;
}

void PrettyPrinter::visit(AST::IfStmt * element) {
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

void PrettyPrinter::visit(AST::OutputStmt * element) {
  os << "output ";
  element->ARG->accept(this);
  os << ";";
}

void PrettyPrinter::visit(AST::ErrorStmt * element) {
  os << "error ";
  element->ARG->accept(this);
  os << ";";
}

void PrettyPrinter::visit(AST::ReturnStmt * element) {
  os << "return ";
  element->ARG->accept(this);
  os << ";";
}

std::string PrettyPrinter::indent() const {
  return std::string(indentLevel*indentSize, indentChar);
}

void PrettyPrinter::safePrintLineNumber(int lineNumber) const {
  if (printLines) {
    os << " // @" << lineNumber;
  }
}
