#include "TipAlpha.h"
#include "TipTypeVisitor.h"
#include "loguru.hpp"
#include <sstream>

TipAlpha::TipAlpha(ASTNode *node) : TipVar(node), context(nullptr), name(""){};

TipAlpha::TipAlpha(ASTNode *node, std::string const name)
    : TipVar(node), context(nullptr), name(name){};

TipAlpha::TipAlpha(ASTNode *node, ASTNode *context, std::string const name)
    : TipVar(node), context(context), name(name){};

std::ostream &TipAlpha::print(std::ostream &out) const {
  out << "\u03B1<" << *node << "@" << node->getLine() << ":"
      << node->getColumn();
  if (context != nullptr) {
    out << "{" << *context << "@" << context->getLine() << ":"
        << context->getColumn() << "}";
  }
  if (name == "") {
    out << ">";
  } else {
    out << "[" << name << "]>";
  }
  return out;
}

bool TipAlpha::operator==(const TipType &other) const {
  auto otherTipAlpha = dynamic_cast<const TipAlpha *>(&other);
  if (!otherTipAlpha) {
    return false;
  }

  return node == otherTipAlpha->getNode() &&
         context == otherTipAlpha->getContext() &&
         name == otherTipAlpha->getName();
}

bool TipAlpha::operator!=(const TipType &other) const {
  return !(*this == other);
}

ASTNode *TipAlpha::getContext() const { return context; }

std::string const &TipAlpha::getName() const { return name; }

void TipAlpha::accept(TipTypeVisitor *visitor) { visitor->endVisit(this); }
