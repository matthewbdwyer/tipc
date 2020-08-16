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

std::set<std::shared_ptr<TipVar>> TipVar::freeVars() const {
    //std::set<std::shared_ptr<TipVar>> fv;
    //fv.emplace(shared_from_this());
    //return std::move(fv);
}

void TipVar::subst(std::shared_ptr<TipVar> v, std::shared_ptr<TipType> t) {
    // TODO
}


