#pragma once
#include "PrettyPrinter.h"
#include "TipType.h"
#include "TypeConstraint.h"
#include <string>

/**
 * Returns human-readable string representations of nodes and types.
 *
 * Currently, Nodes do not have a friendly intrinsic representation. This
 * makes the output of printing them, types, and constraints not very useful. To
 * generate more readable output, you can use this helper class.
 *
 * NB: Types and TypeConstraints are all setup to have human-readable output. Once,
 * Nodes have their own friendly output this class should be removed.
 */
class Stringifier {
public:
    static std::string stringify(ASTNode * node);
    static std::string stringify(TipType * tipType);
    static std::string stringify(TypeConstraint * constraint);
};

