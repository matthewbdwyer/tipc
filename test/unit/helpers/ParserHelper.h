#pragma once

#include <istream>

class ParserHelper {
public:
  static bool is_parsable(std::istream &stream);
  static std::string parsetree(std::istream &stream);
};

