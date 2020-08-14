#pragma once

#include "ASTProgram.h"
#include "SymbolTable.h"

/*! \class TypeInference
 *  \brief Perform type inference and checking.
 *
 * This class provides the inferandcheck method to run type inference/checking on a 
 * given program.
 *
 * It currently does not compute the mappig from expressions to inferred types, but
 * it could be extended to do so.
 */
class TypeInference {
public:
  /*! \fn inferandcheck
   *  \brief Generate type constraints, unify them, and report any errors.
   *
   * Visits the AST generating type constraints for each expression and solves
   * the resulting constraints.   If a term unification error is detected a
   * UnificationError, a subtype fo SemanticError, is raised.
   * \sa UnificationError
   * \sa SemanticError
   * \param ast The program AST
   * \param symbols The symbol table
   */
  static void inferandcheck(ASTProgram* ast, SymbolTable* symbols); 
};
