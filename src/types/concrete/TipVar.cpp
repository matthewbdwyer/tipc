#include "TipVar.h"
#include <sstream>
#include <iostream>

TipVar::TipVar(ASTNode * node): node(node) {};

bool TipVar::operator==(const TipType &other) const {
    if(auto t = dynamic_cast<TipVar const *>(&other)) {
        return node == t->node;
    }
    return false;
}

bool TipVar::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::ostream &TipVar::print(std::ostream &out) const {
    out << "[[" << node << "]]";
    return out;
}
