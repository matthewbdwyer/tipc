#pragma once

#include "Error.h"

/*! \class SemanticError
 *  \brief Class for tipc semantic errors.
 *
 * Errors in weeding, symbol processing, or type checking generate instances of this type.
 */
class SemanticError: public Error {
public:
  SemanticError(const std::string& msg) : Error(msg) {} //! Construct semantic error with message
};


