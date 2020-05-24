#pragma once

#include "TIPtreeBuild.h"
#include <istream>

/**
 * A collection of useful helper methods for testing.
 */
class GeneralHelper {
public:
  static std::vector<std::string> tokenize(std::string str, char delim);
};


