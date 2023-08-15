#pragma once

#include "ASTNode.h"
#include "ASTProgram.h"
#include "SymbolTable.h"
#include "TypeInference.h"
#include "cfa/CallGraph.h" //call graph builder header
#include <memory>

/*! \class SemanticAnalysis
 *  \brief Stores the results of semantic analysis passes.
 *
 * This class provides the analyze method to run a set of semantic analyses,
 * including l-value checking for assignment statements, proper use of symbols,
 * and type checking and control flow analysis \sa SymbolTable \sa TypeInference
 * \sa CallGraph
 */
class SemanticAnalysis {
  std::shared_ptr<SymbolTable> symTable;
  std::shared_ptr<TypeInference> typeResults;
  std::shared_ptr<CallGraph> callGraph;

public:
  SemanticAnalysis(std::shared_ptr<SymbolTable> s,
                   std::shared_ptr<TypeInference> t,
                   std::shared_ptr<CallGraph> cg)
      : symTable(std::move(s)), typeResults(std::move(t)),
        callGraph(std::move(cg)) {}

  /*! \fn analyze
   *  \brief Perform semantic analysis on program AST.
   *
   * Run weeding, symbol, and type checking and control flow analysis.  Errors
   * in any of these result in a SemanticError.  If no errors then ownership of
   * semantic analysis results are transferred to caller. \sa SemanticError
   * \param ast The program AST
   * \param polyInf Indicate whether polymorphic type inference should be
   * performed. \return The unique pointer to the semantic analysis structure.
   */
  static std::shared_ptr<SemanticAnalysis> analyze(ASTProgram *ast,
                                                   bool polyInf);

  /*! \fn getSymbolTable
   *  \brief Returns the symbol table computed for the program.
   * \sa SymbolTable
   */
  SymbolTable *getSymbolTable();

  /*! \fn getTypeResults
   *  \brief Returns the type inference results.
   * \sa TypeInference
   */
  TypeInference *getTypeResults();

  /*! \fn getCallGraph
   *  \brief Returns the call graph for the program.
   * \sa CallGraph
   */
  CallGraph *getCallGraph();
};
