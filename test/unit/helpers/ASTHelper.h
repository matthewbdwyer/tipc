#pragma once

#include "ASTBuilder.h"
#include "ASTNodeFinder.h"

#include <sstream>

namespace ASTHelper {
    std::shared_ptr<ASTProgram> build_ast(std::stringstream &stream);

    template<class N> std::shared_ptr<N> find_node(std::shared_ptr<ASTProgram> p) {
        return NodeFinder<N>::find_node(p.get());
    }
};
