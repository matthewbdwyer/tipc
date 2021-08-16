#pragma once

#include "ASTVisitor.h"
#include <map>
#include <vector>

/*! \class SymbolTable
 *  \brief Performs symbol analysis and records results for subsequent phases.
 *
 * The symbol table maps names of identifiers to declaration nodes.
 * There is a global map of for function names and a local map for
 * each function.  In addition it records the set of field names used
 * in the program.  Errors are reported by raising a SemanticError exception.
 * \sa SemanticError
 */ 
class SymbolTable {
  std::map<std::string, ASTDeclNode*> functionNames;
  std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> localNames;
  std::vector<std::string> fieldNames;
public:
  SymbolTable(std::map<std::string, ASTDeclNode*> fMap,
              std::map<ASTDeclNode*, std::map<std::string, ASTDeclNode*>> lMap,
              std::vector<std::string> fSet)
      : functionNames(fMap), localNames(lMap), fieldNames(fSet) {}

  /*! \brief Return the declaration node for a given function name.
   * \param s The Function name
   * \return The declaration node of the function
   */
  ASTDeclNode* getFunction(std::string s);

  /*! \brief Return the declaration nodes for functions in the program.
   */
  std::vector<ASTDeclNode*> getFunctions();

  /*! \brief Return the declaration node for local or a parameter in a function.
   * \param s The local or parameter name
   * \param f The declaration node of the function
   * \return The declaration node of the local or parameter
   */
  ASTDeclNode* getLocal(std::string s, ASTDeclNode* f);

  /*! \brief Return the declaration nodes for locals and parameters in a function.
   * \param f The declaration node of the function.
   */
  std::vector<ASTDeclNode*> getLocals(ASTDeclNode* f);

  /*! \brief Returns the record field names referenced in the program.
   */
  std::vector<std::string> getFields();

  /*! \fn build
   *  \brief Perform symbol analysis and construct symbol table.
   *
   * Errors are reported by raising a SemanticError exception.
   * \param p The AST for the program. 
   * \return The symbol table.
   */
  static std::unique_ptr<SymbolTable> build(ASTProgram* p);

  //! Print symbol table contents to output stream
  void print(std::ostream &os);
};

