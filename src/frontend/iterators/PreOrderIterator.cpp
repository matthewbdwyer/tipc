#include "PreOrderIterator.h"
#include <iostream>

PreOrderIterator::PreOrderIterator(const SyntaxTree &tree, bool end) : IteratorImpl(tree) {
  if(!end) {
    stack.push(tree);
  }
}

PreOrderIterator::~PreOrderIterator() = default;

SyntaxTree PreOrderIterator::operator*() {
  return stack.top();
}

SyntaxTree *PreOrderIterator::operator->() {
  return &stack.top();
}

bool PreOrderIterator::operator==(const IteratorImpl &rhs) const {
  return IteratorUtils::are_equal(this, rhs);
}

bool PreOrderIterator::operator!=(const IteratorImpl &rhs) const {
  return !(*this == rhs);
}

void PreOrderIterator::operator++() {
  if(stack.empty()) {
    return;
  }

  SyntaxTree current = stack.top();
  stack.pop();

  auto subtrees = current.getSubtrees();
  if(subtrees.empty()) {
    return;
  }

  for(int i = subtrees.size() - 1; i >= 0; i--) {
    stack.push(subtrees.at(i));
  }
}

IteratorImpl * PreOrderIterator::clone() {
  return new PreOrderIterator(*this);
}  // LCOV_EXCL_LINE

