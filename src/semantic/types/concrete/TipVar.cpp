#include "TipVar.h"
#include "TipAlpha.h"
#include "TipTypeVisitor.h"

#include <sstream>
#include <iostream>

TipVar::TipVar(ASTNode * node): node(node) {};

bool TipVar::operator==(const TipType &other) const {
    auto otherTipVar = dynamic_cast<TipVar const *>(&other);
    auto otherTipAlpha = dynamic_cast<TipAlpha const *>(&other);
    if(!otherTipVar || otherTipAlpha) {
        return false;
    }

    return node == otherTipVar->getNode();
}

bool TipVar::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::ostream &TipVar::print(std::ostream &out) const {
    out << "[[" << *node << "@" << node->getLine() << ":" << node->getColumn() << "]]";
    return out;
}

void TipVar::accept(TipTypeVisitor * visitor) {
  visitor->visit(this);
  visitor->endVisit(this);
}
