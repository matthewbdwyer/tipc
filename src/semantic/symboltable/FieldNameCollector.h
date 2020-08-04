#pragma once

#include "ASTVisitor.h"
#include <set>
#include <string>

/*! \class FieldNameCollector
 *  \brief Collects all field names referenced within the program.
 *
 * This class is a minimal visitor that collects field names referenced
 * in record expressions, as field sub-expression, and in access expressions.
 */
class FieldNameCollector :  public ASTVisitor {
    std::vector<std::string> fields;
public:
    FieldNameCollector() = default;
    static std::vector<std::string> build(ASTProgram* p);
    virtual void endVisit(ASTFieldExpr * element) override;
    virtual void endVisit(ASTAccessExpr * element) override;
};

