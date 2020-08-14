#pragma once

#include "ASTProgram.h"
#include "SymbolTable.h"
#include <memory>

/*! \class SemanticAnalysis
 *  \brief Stores the results of semantic analysis passes.
 *
 * This class provides the analyze method to run a set of semantic analyses, including
 * l-value checking for assignment statements, proper use of symbols, and type checking.   
 * \sa SymbolTable
 *
 * The class does not currently record the inferred types for each expression in
 * the program, but this could be added as an extension.
 */
class SemanticAnalysis {
  std::unique_ptr<SymbolTable> symTable;

public:
  SemanticAnalysis(std::unique_ptr<SymbolTable> symTable) : symTable(std::move(symTable)) {}

  /*! \fn analyze
   *  \brief Perform semantic analysis on program AST.
   *
   * Run weeding, symbol, and type checking analysis.  Errors in any of these
   * result in a SemanticError.  If no errors then ownership of semantic analysis
   * results are transfered to caller.
   * \sa SemanticError
   * \param ast The program AST
   * \return The unique pointer to the semantic analysis structure.
   */
  static std::unique_ptr<SemanticAnalysis> analyze(ASTProgram* ast); 

  SymbolTable* getSymbolTable(); 
};
