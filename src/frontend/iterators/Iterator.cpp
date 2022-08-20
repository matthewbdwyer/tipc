#include "Iterator.h"

Iterator::Iterator(const Iterator &ts): iteratorImpl(ts.iteratorImpl) {

}

Iterator::Iterator(IteratorImpl *impl): iteratorImpl(impl) {

}

SyntaxTree Iterator::operator*() {
  return *(*iteratorImpl);
}

Iterator &Iterator::operator++() {
  ++(*iteratorImpl);
  return *this;
}

Iterator Iterator::operator++(int) {
  Iterator cloned(iteratorImpl->clone());
  ++(*iteratorImpl);
  return cloned;
}

SyntaxTree *Iterator::operator->() {
  return iteratorImpl->operator->();
}

bool Iterator::operator==(const Iterator &rhs) {
  return *iteratorImpl == *rhs.iteratorImpl;
}

bool Iterator::operator!=(const Iterator &rhs) {
  return !(*this == rhs);
}

SyntaxTree const &Iterator::get_tree() const {
  return iteratorImpl->get_tree();
}
