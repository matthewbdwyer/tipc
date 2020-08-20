#pragma once

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <memory>
#include <string>
#include <vector>
#include <ostream>

// Forward declare the visitor to resolve circular dependency
class ASTVisitor;

/*! \brief Abstract base class for all AST nodes.
 *
 * ASTNodes define the elements of the program in a tree structured form.
 * A key concept to undestand is that of "ownership".   Every node in the
 * tree is "owned" by its parent.  The parent maintains a "unique pointer"
 * to each of its children.  This greatly simplifies memory management as
 * the tree can be reclaimed once its root is no longer needed.  It is 
 * important to understand that the "unique pointer" is only used to manage
 * the lifetime of the child nodes.  The values of those pointers can be
 * passed around freely to other parts of the compiler that want to view
 * the ASTNodes.
 *
 * Each node has information about the line and column in the source
 * program on which the element begins.  Subtypes define "<<" operator
 * that print a custom representation of the ASTNode, by overriding
 * the print method.
 *
 * There are two virtual methods that are used in a generic visitor
 * and for code generation pass. 
 */
class ASTNode {
  int line = 0;
  int column = 0;
public:
  virtual ~ASTNode() = default;

  /*! \fn accept
   *  \brief Visit the children of this node and apply the visitor.
   *
   * This virtual function is overridden by each ASTNode subtype
   * in order to visit each of its child nodes, by calling their accept
   * methods.  The visitor parameter defines the operations that are
   * applied to each node that is visited; a subtype of ASTVisitor defines
   * those operations.
   *
   * \param visitor The subtype of ASTVisitor that carries out per-ASTNode work.
   */
  virtual void accept(ASTVisitor * visitor) = 0;
  
  /*! \fn codegen
   *  \brief Perform code generation and return an LLVM value the code.
   *
   * This virtual function is overridden by each ASTNode subtype in order
   * to generate the executable code.  This function does not use the visitor
   * due to the fact that a high-degree of control on the ordering of the 
   * nodes is required.
   *
   * \return LLVM value holding an representation of the generated code.
   */
  virtual llvm::Value* codegen() = 0;

  void setLocation(int l, int c) { line = l; column = c; }
  int getLine() { return line; }
  int getColumn() { return column; }

  friend std::ostream& operator<<(std::ostream& os, const ASTNode& obj) {
    return obj.print(os);
  }

protected:
  virtual std::ostream& print(std::ostream &out) const = 0;
};
