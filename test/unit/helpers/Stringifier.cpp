#include "Stringifier.h"
#include "TipInt.h"
#include "TipRef.h"
#include "TipFunction.h"
#include "TipVar.h"
#include "TipAlpha.h"
#include "TipRecord.h"
#include "PrettyPrinter.h"
#include <sstream>


std::string Stringifier::stringify(ASTNode * node) {
    std::stringstream stream;
    PrettyPrinter prettyPrinter(stream, ' ', 2);
    prettyPrinter.print2(node, stream, ' ', 2);
    return stream.str();
}

std::string Stringifier::stringify(TipType * tipType) {
    std::stringstream stream;

    // The order of this test is very important.  Subtypes must precede supertypes.
    if(auto n = dynamic_cast<TipInt *>(tipType)) {
        stream << *n;
    } else if(auto n = dynamic_cast<TipRef *>(tipType)) {
        stream << "&" << stringify(n->of.get());
    } else if(auto n = dynamic_cast<TipAlpha *>(tipType)) {
        stream << *n;
    } else if(auto n = dynamic_cast<TipFunction *>(tipType)) {
        stream << "(";
        bool first = true;
        std::stringstream params;
        for(auto &param : n->params) {
            if(first) {
                params << stringify(param.get());
                first = false;
                continue;
            }
            params << ", " << stringify(param.get());
        }
        stream << params.str() << ") -> " << stringify(n->ret.get());
    } else if(auto n = dynamic_cast<TipVar *>(tipType)) {
        stream << "[[" << stringify(n->node) << "]]";
    } else if(auto n = dynamic_cast<TipRecord *>(tipType)) {
        stream << "{";
        bool first = true;
        std::stringstream fieldinit;
        int i = 0;
        for(auto &init : n->inits) {
            if(first) {
                fieldinit << n->names.at(i++) << ":" << stringify(init.get());
                first = false;
                continue;
            }
            fieldinit << ", " << n->names.at(i++) << ":" << stringify(init.get());
        }
        stream << fieldinit.str() << "}";

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
