#pragma once
#include <string>
#include "IteratorImpl.h"


#include "SyntaxTree.h"

/*! \brief The client-facing iterator interface.
 *
 * `Iterator` is the interface STL calls like `begin` expect you to work with. Here,
 * we delegate calls to the given `IteratorImpl` to abstract away design details. This
 * technique is commonly called, "Pointer to implementation" or "pImpl" by C++ programmers.
 * If you are more familiar with GOF design patterns, think of this as the Bridge Pattern.
 */
class Iterator {

public:
  Iterator(const Iterator &ts);

  Iterator(IteratorImpl *impl);

  SyntaxTree operator* ();

  Iterator &operator++ ();

  Iterator operator++ (int);

  SyntaxTree *operator-> ();

  bool operator== (const Iterator &rhs);

  bool operator!= (const Iterator &rhs);

  SyntaxTree const & get_tree() const;


private:
  std::shared_ptr<IteratorImpl> iteratorImpl;
};

