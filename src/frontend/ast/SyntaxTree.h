#pragma once

#include "treetypes/ASTNode.h"

#include <memory>

// Forward declare the iterator.
class Iterator;

/*! \brief A convenient interface to the AST nodes.
 *
 * The SyntaxTree abstraction provides an interface for working with the AST.
 * It shelters clients from having to learn low-level details about the nodes. For
 * instance, STL like iterators are provided that handle traversal for you.
 */
class SyntaxTree {

public:
  SyntaxTree(std::shared_ptr<ASTNode> node);
  ~SyntaxTree();
  SyntaxTree(SyntaxTree const &tree);
  SyntaxTree & operator=(SyntaxTree const &tree);

  /*! \fn begin
   *  \brief Construct an iterator pointing at the root of the tree.
   *
   * \param order the traversal order. Currently, only pre-order is supported.
   */
  Iterator begin(std::string const &order);

  /*! \fn end
   *  \brief Construct an iterator pointing at the end of the tree.
   *
   * \param order the traversal order. Currently, only pre-order is supported.
   */
  Iterator end(std::string const &order);


  /*! \fn getRoot
   *  \brief Return the root of the tree.
   */
  std::shared_ptr<ASTNode> getRoot() const;

  /*! \fn accept
   *  \brief Apply the visitor to the root of the tree.
   *
   *  \param visitor The visitor to apply.
   */
  void accept(ASTVisitor * visitor);

  /*! \fn getSubtrees
   *  \brief Return the children of the root as collection of SyntaxTrees.
   */
  std::vector<SyntaxTree> getSubtrees();

private:
  std::shared_ptr<ASTNode> root;


};


