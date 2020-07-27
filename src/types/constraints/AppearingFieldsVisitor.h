#pragma once
#include "ASTVisitor.h"
#include <set>
#include <string>


// TODO (nphair): The symbol table seems like a better place for this logic.
class AppearingFieldsVisitor:  public ASTVisitor {
public:
    AppearingFieldsVisitor() = default;
    virtual void endVisit(AST::FieldExpr * element) override;
    std::set<std::string>& getFields();

private:
    std::set<std::string> fields;
};

