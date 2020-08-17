#pragma once

#include "ASTVisitor.h"
#include "ConstraintHandler.h"
#include "SymbolTable.h"
#include "TipType.h"
#include <memory>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include "TypeConstraintVisitor.h"
#include "TypeConstraint.h"

/*! \class TypeConstraintCollectVisitor
 *
 *  \brief Visitor generates type constraints and collects them.
 */
class TypeConstraintCollectVisitor: public TypeConstraintVisitor {
public:
    explicit TypeConstraintCollectVisitor(SymbolTable *pTable);
    std::vector<TypeConstraint>& getCollectedConstraints();
private:
    static std::unique_ptr<ConstraintHandler> buildConstraintHandler();
};

