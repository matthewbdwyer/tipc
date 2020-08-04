#pragma once

#include "Error.h"

/*! \class ParseError
 *  \brief Class for tipc parse errors.
 *
 * Errors in lexing or parsing generate instances of this type.
 */
class ParseError: public Error {
public:
  ParseError(const std::string& msg) : Error(msg) {} //! Construct parse error with message
};


