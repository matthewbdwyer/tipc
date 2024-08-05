#pragma once

#include "memory"
#include <vector>

#include <TipType.h>
#include <AST.h>

class TypeHelper {
public:
    static std::shared_ptr<TipType> intType();
    static std::shared_ptr<TipType> alphaType(ASTNode *node);
    static std::shared_ptr<TipType> ptrType(std::shared_ptr<TipType> t);
    static std::shared_ptr<TipType> funType(std::vector<std::shared_ptr<TipType>> p, std::shared_ptr<TipType> r);
    static std::shared_ptr<TipType> recType(std::vector<std::shared_ptr<TipType>> t, std::vector<std::string> n);
    static std::shared_ptr<TipType> absentType();
};
