#pragma once

#include "SemanticError.h"

/*!
 * \class UnificationError
 *
 * \brief A runtime error when unifying type terms.
 */
class UnificationError: public SemanticError {
public:
    UnificationError(const char * msg) : SemanticError(msg) {};
};


