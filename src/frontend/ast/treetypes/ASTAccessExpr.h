#pragma once

#include "ASTExpr.h"

//! \brief Class for a record field access
class ASTAccessExpr : public ASTExpr {
  std::unique_ptr<ASTExpr> RECORD;
  std::string FIELD;
public:
  ASTAccessExpr(std::unique_ptr<ASTExpr> RECORD, const std::string &FIELD)
      : RECORD(std::move(RECORD)), FIELD(FIELD) {}
  std::string getField() { return FIELD; }
  ASTExpr* getRecord() { return RECORD.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;
};
