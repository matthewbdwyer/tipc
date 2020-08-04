#pragma once

#include "SemanticError.h"

class UnificationError: public SemanticError {
public:
    UnificationError(const char * msg) : SemanticError(msg) {};
};


