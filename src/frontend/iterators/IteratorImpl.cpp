#include "IteratorImpl.h"

IteratorImpl::IteratorImpl(const SyntaxTree &tree) : tree(tree) {}

SyntaxTree const & IteratorImpl::get_tree() const {
  return tree;
}

IteratorImpl::~IteratorImpl() = default;