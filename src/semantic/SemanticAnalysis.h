#pragma once

#include "ASTProgram.h"
#include "ASTNode.h"
#include "SymbolTable.h"
#include "TypeInference.h"
#include <memory>
#include "cfa/CallGraph.h"  //call graph builder header

/*! \class SemanticAnalysis
 *  \brief Stores the results of semantic analysis passes.
 *
 * This class provides the analyze method to run a set of semantic analyses, including
 * l-value checking for assignment statements, proper use of symbols, and type checking and control flow analysis
 * \sa SymbolTable
 * \sa TypeInference
 * \sa CallGraph
 */
class SemanticAnalysis {
  std::unique_ptr<SymbolTable> symTable;
  std::unique_ptr<TypeInference> typeResults;
  std::unique_ptr<CallGraph> callGraph;


public:
  SemanticAnalysis(std::unique_ptr<SymbolTable> s, std::unique_ptr<TypeInference> t, std::unique_ptr<CallGraph> cg)
          : symTable(std::move(s)), typeResults(std::move(t)), callGraph(std::move(cg)) {}

  /*! \fn analyze
   *  \brief Perform semantic analysis on program AST.
   *
   * Run weeding, symbol, and type checking and control flow analysis.  Errors in any of these
   * result in a SemanticError.  If no errors then ownership of semantic analysis
   * results are transferred to caller.
   * \sa SemanticError
   * \param ast The program AST
   * \return The unique pointer to the semantic analysis structure.
   */
  static std::unique_ptr<SemanticAnalysis> analyze(ASTProgram* ast); 

  /*! \fn getSymbolTable
   *  \brief Returns the symbol table computed for the program.
   * \sa SymbolTable
   */
  SymbolTable* getSymbolTable(); 

  /*! \fn getTypeResults
   *  \brief Returns the type inference results.
   * \sa TypeInference
   */
  TypeInference* getTypeResults();

  /*! \fn getCallGraph
  *  \brief Returns the call graph for the program.
  * \sa CallGraph
  */
  CallGraph* getCallGraph();

};
