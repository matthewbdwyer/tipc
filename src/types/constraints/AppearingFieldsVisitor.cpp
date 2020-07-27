#include "AppearingFieldsVisitor.h"

void AppearingFieldsVisitor::endVisit(ASTFieldExpr * element) {
    fields.emplace(element->getField());
}

std::set<std::string>& AppearingFieldsVisitor::getFields() {
    return fields;
}
