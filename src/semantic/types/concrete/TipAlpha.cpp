#include "TipAlpha.h"
#include "TipTypeVisitor.h"

#include <sstream>

TipAlpha::TipAlpha(ASTNode* node): TipVar(node), name("") {};

TipAlpha::TipAlpha(ASTNode* node, std::string const name): TipVar(node), name(name) {};

std::ostream &TipAlpha::print(std::ostream &out) const {
    if (name == "") {
      out << "\u03B1<" << *node << ">";
    } else {
      out << "\u03B1<" << *node << ":" << name << ">";
    }
    return out;
}

bool TipAlpha::operator==(const TipType &other) const {
    auto otherTipAlpha = dynamic_cast<const TipAlpha *>(&other);
    if(!otherTipAlpha) {
        return false;
    }

    return node == otherTipAlpha->getNode() && name == otherTipAlpha->getName();
}

bool TipAlpha::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::string const & TipAlpha::getName() const {
    return name;
}

void TipAlpha::accept(TipTypeVisitor * visitor) {
  visitor->visit(this);
  visitor->endVisit(this);
}

