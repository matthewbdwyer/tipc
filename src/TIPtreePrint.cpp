#include "TIPtree.h"

namespace TIPtree {

static std::string indent;
static int indentLevel = 0;
static bool printLines = false;
static bool printTypes = false;

// indentation is the contatenation of "level" indent strings
std::string indentation() {
  std::string indStr;
  for (int i = 0; i < indentLevel; i++) {
    indStr += indent;
  }
  return indStr;
}

std::string Program::print(std::string i, bool pl = false, bool pt = false) {
  indent = i;      // initialize namespace global for indent stride
  printLines = pl; // print line numbers
  printTypes = pt;
  std::string pp;
  for (auto const &fn : FUNCTIONS) {
    pp += fn->print() + "\n";
  }
  return pp;
}

std::string Function::print() {
  std::string pp = NAME + "(";

  // comma separated parameter name list
  bool skip = true;
  for (auto param : FORMALS) {
    if (skip) {
      skip = false;
      pp += param;
    } else {
      pp += ", " + param;
    }
  }
  pp += ")";

  if (printTypes) {
    pp += ": " + type->print();
  }

  if (printLines) {
    pp += " // @" + std::to_string(LINE);
  }
  pp += "\n";

  pp += indentation() + "{\n";
  indentLevel++;

  for (auto const &decl : DECLS) {
    pp += indentation() + decl->print() + "\n";
  }

  for (auto const &stmt : BODY) {
    pp += indentation() + stmt->print() + "\n";
  }

  indentLevel--;
  pp += indentation() + "}\n";

  return pp;
}

std::string NumberExpr::print() { return std::to_string(VAL); }

std::string VariableExpr::print() { return NAME; }

std::string BinaryExpr::print() {
  return "(" + LHS->print() + " " + OP + " " + RHS->print() + ")";
}

std::string InputExpr::print() { return "input"; }

std::string FunAppExpr::print() {
  std::string pp = FUN->print() + "(";
  // comma separated argument list
  bool skip = true;
  for (auto const &arg : ACTUALS) {
    if (skip) {
      skip = false;
      pp += arg->print();
    } else {
      pp += ", " + arg->print();
    }
  }
  pp += ")";
  return pp;
}

std::string AllocExpr::print() { return "alloc " + ARG->print(); }

std::string RefExpr::print() { return "(&" + NAME + ")"; }

std::string DeRefExpr::print() { return "(*" + ARG->print() + ")"; }

std::string NullExpr::print() { return "null"; }

std::string FieldExpr::print() { return FIELD + ":" + INIT->print(); }

std::string RecordExpr::print() {
  std::string pp = "{";
  // comma separated argument list
  bool skip = true;
  for (auto const &field : FIELDS) {
    if (skip) {
      skip = false;
      pp += field->print();
    } else {
      pp += ", " + field->print();
    }
  }
  pp += "}";
  return pp;
}

std::string AccessExpr::print() { return RECORD->print() + "." + FIELD; }

std::string DeclStmt::print() {
  std::string pp = "var ";
  // comma separated variable names list
  bool skip = true;
  auto typeIt = types.begin();

  for (auto id : VARS) {
    if (skip) {
      skip = false;
      pp += id;
    } else {
      pp += ", " + id;
    }

    if (printTypes && typeIt != types.end()) {
      pp += ": " + (*typeIt)->print();
    }
    typeIt++;
  }
  pp += ";";
  if (printLines)
    pp += " // @" + std::to_string(LINE);
  return pp;
}

std::string AssignStmt::print() {
  return LHS->print() + " = " + RHS->print() + ";";
}

std::string BlockStmt::print() {
  std::string pp = "{\n";
  indentLevel++;
  for (auto const &s : STMTS) {
    pp += indentation() + s->print() + "\n";
  }
  indentLevel--;
  pp += indentation() + "}";
  return pp;
}

std::string WhileStmt::print() {
  std::string pp = "while (" + COND->print() + ") ";
  pp += BODY->print();
  return pp;
}

std::string IfStmt::print() {
  std::string pp = "if (" + COND->print() + ") ";
  pp += THEN->print();

  if (ELSE != nullptr) {
    pp += " else " + ELSE->print();
  }
  return pp;
}

std::string OutputStmt::print() { return "output " + ARG->print() + ";"; }

std::string ErrorStmt::print() { return "error " + ARG->print() + ";"; }

std::string ReturnStmt::print() { return "return " + ARG->print() + ";"; }

} // namespace TIPtree
