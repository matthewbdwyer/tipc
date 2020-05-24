#pragma once

#include "TIPtreeBuild.h"
#include <istream>

class ASTHelper {
public:
  static std::unique_ptr<TIPtree::Program> build_ast(std::istream &stream);
};

