#pragma once
#include "IteratorImpl.h"
#include "SyntaxTree.h"
#include <stack>

/*! \brief An iterator implementation for a depth-first, pre-order, transversal.
 */
class PreOrderIterator: public IteratorImpl {

public:
  explicit PreOrderIterator(SyntaxTree const &tree, bool end);

  ~PreOrderIterator() override;

  SyntaxTree operator*() override;

  void operator++() override;

  SyntaxTree *operator->() override;

  bool operator==(const IteratorImpl &rhs) const override;

  bool operator!=(const IteratorImpl &rhs) const override;

  IteratorImpl * clone() override;

//private:
  std::stack<SyntaxTree> stack;

};

#include "IteratorUtils.tpp"
