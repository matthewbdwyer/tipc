#include "FieldNameCollector.h"

#include <algorithm>

void FieldNameCollector::endVisit(ASTFieldExpr * element) {
  if (std::find(fields.begin(), fields.end(), element->getField()) == fields.end()) {
    fields.push_back(element->getField());
  }
}

void FieldNameCollector::endVisit(ASTAccessExpr * element) {
  if (std::find(fields.begin(), fields.end(), element->getField()) == fields.end()) {
    fields.push_back(element->getField());
  }
}

std::vector<std::string> FieldNameCollector::build(ASTProgram* p) {
    FieldNameCollector visitor;
    p->accept(&visitor);
    return visitor.fields;
}
