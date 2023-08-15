#pragma once

#include "ASTDeclNode.h"
#include "ASTProgram.h"
#include "CallGraph.h"
#include "SymbolTable.h"
#include "Unifier.h"
#include <memory>

/*! \class TypeInference
 *  \brief Perform type inference and checking.
 *
 * This class provides the check method to run type inference/checking on a
 * given program.  The inferred types for names declared in the program can then
 * be accessed.
 */
class TypeInference {
  SymbolTable *symbols;

  // This unifier stores either the monomorphic or polymorphic type inference
  // results
  std::shared_ptr<Unifier> unifier;

public:
  TypeInference(SymbolTable *s, std::shared_ptr<Unifier> u)
      : symbols(s), unifier(u) {}

  /*! \fn run
   *  \brief Generate and solve type constraints and report any errors.
   *
   * Visits the AST generating type constraints for each expression and solves
   * the resulting constraints.   If a term unification error is detected a
   * UnificationError, a subtype of SemanticError, is raised.  If an access
   * to a field whose definition is absent is detected a SemanticError is
   * raised.  If no errors are found, then the inferred types for all symbols
   * are recorded in the symbol table.
   *
   * \sa UnificationError
   * \sa SemanticError
   * \param ast The program AST
   * \param polyInf Flag indicating whether to perform polymorphic or
   * monomorphic inference \param cg The program call graph \param symbols The
   * symbol table
   */
  static std::shared_ptr<TypeInference>
  run(ASTProgram *ast, bool polyInf, CallGraph *cg, SymbolTable *symbols);

  /*! \fn getInferredType
   *  \brief Returns the type expression inferred for the given ASTDeclNode.
   *
   * After type checking completes the inferred types can be accessed for
   * declared names. Note that it is possible to declare a variable, but never
   * use it, in which case the inferred type will be a free type variable.  A
   * managed pointer is returned for exactly this case -- when a fresh variable
   * is generated and returned.  In other cases, the resulting type will be
   * shared with those computed during inference -- hence a shared pointer.
   *
   * \sa TipType
   * \sa ASTDeclNode
   * \param node An AST declaration node.
   * \return A shared pointer to the inferred type for the AST node.
   */
  std::shared_ptr<TipType> getInferredType(ASTDeclNode *node);

  //! Print type inference results to output stream
  void print(std::ostream &os);
};
