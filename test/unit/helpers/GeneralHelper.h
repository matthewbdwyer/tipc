#pragma once

#include "ASTBuilder.h"

#include <istream>

/**
 * A collection of useful helper methods for testing.
 */
class GeneralHelper {
public:
  static std::vector<std::string> tokenize(std::string str, char delim);
  static std::string removeTrailingWhitespace(std::string);
  static int countSubstrings(std::string str, std::string substr);
};


