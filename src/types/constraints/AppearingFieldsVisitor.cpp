#include "AppearingFieldsVisitor.h"

void AppearingFieldsVisitor::endVisit(AST::FieldExpr * element) {
    fields.emplace(element->getField());
}

std::set<std::string>& AppearingFieldsVisitor::getFields() {
    return fields;
}
