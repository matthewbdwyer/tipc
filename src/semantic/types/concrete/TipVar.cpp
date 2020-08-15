#include "TipVar.h"
#include <sstream>
#include <iostream>

TipVar::TipVar(ASTNode * node): node(node) {};

bool TipVar::operator==(const TipType &other) const {
    auto otherTipVar = dynamic_cast<TipVar const *>(&other);
    if(!otherTipVar) {
        return false;
    }

    return node == otherTipVar->node;
}

bool TipVar::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::ostream &TipVar::print(std::ostream &out) const {
    out << "[[" << *node << "@" << node->getLine() << ":" << node->getColumn() << "]]";
    return out;
}
