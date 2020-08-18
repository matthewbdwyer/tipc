#include "Stringifier.h"
#include "TipInt.h"
#include "TipRef.h"
#include "TipFunction.h"
#include "TipVar.h"
#include "TipAlpha.h"
#include "TipRecord.h"
#include <sstream>


std::string Stringifier::stringify(ASTNode * node) {
    std::stringstream stream;
    stream << *node;
    return stream.str();
}

std::string Stringifier::stringify(TipType * tipType) {
    std::stringstream stream;

    // The order of this test is very important.  Subtypes must precede supertypes.
    if(auto n = dynamic_cast<TipInt *>(tipType)) {
        stream << *n;
    } else if(auto n = dynamic_cast<TipRef *>(tipType)) {
        stream << *n;
    } else if(auto n = dynamic_cast<TipAlpha *>(tipType)) {
        stream << *n;
    } else if(auto n = dynamic_cast<TipFunction *>(tipType)) {
        stream << *n;
    } else if(auto n = dynamic_cast<TipVar *>(tipType)) {
        stream << *n;
    } else if(auto n = dynamic_cast<TipRecord *>(tipType)) {
        stream << *n;
    } else {
        assert(0);
    }

    return stream.str();
}

std::string Stringifier::stringify(TypeConstraint * constraint) {
    std::stringstream stream;
    stream << stringify(constraint->lhs.get())
              << " = "
              << stringify(constraint->rhs.get());
    return stream.str();
}
