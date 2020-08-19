#include "Stringifier.h"
#include <sstream>

std::string Stringifier::stringify(ASTNode * node) {
    std::stringstream stream;
    stream << *node;
    return stream.str();
}

std::string Stringifier::stringify(TipType * tt) {
    std::stringstream stream;
    stream << *tt;
    return stream.str();
}

std::string Stringifier::stringify(TypeConstraint * constraint) {
    std::stringstream stream;
    stream << stringify(constraint->lhs.get())
              << " = "
              << stringify(constraint->rhs.get());
    return stream.str();
}
