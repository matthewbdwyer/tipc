#pragma once

#include "ASTVisitor.h"
#include <set>
#include <string>

class FieldNameCollector :  public ASTVisitor {
    std::vector<std::string> fields;
public:
    FieldNameCollector() = default;
    static std::vector<std::string> build(ASTProgram* p);
    virtual void endVisit(ASTFieldExpr * element) override;
};

