#pragma once

#include "ASTBuilder.h"

#include <istream>

class ASTHelper {
public:
  static std::shared_ptr<ASTProgram> build_ast(std::istream &stream);
};
