#pragma once
#include "ASTNode.h"
#include "TipType.h"
#include "TypeConstraint.h"
#include <string>

/**
 * Returns human-readable string representations of nodes and types.
 */
class Stringifier {
public:
    static std::string stringify(ASTNode * node);
    static std::string stringify(TipType * tipType);
    static std::string stringify(TypeConstraint * constraint);
};

