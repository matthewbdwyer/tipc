#pragma once

#include "ASTBuilder.h"

#include <istream>

class ASTHelper {
public:
  static std::unique_ptr<ASTProgram> build_ast(std::istream &stream);
};

