#pragma once

#include <TipType.h>
#include <iostream>
#include <map>
#include <vector>

/*!
 * \class UnionFind
 *
 * \brief Specialized implementation of a union-find data structure tailored to
 * work with TipTypes wrapped in shared pointers.
 */
class UnionFind {
public:
  UnionFind() = default;
  explicit UnionFind(std::vector<std::shared_ptr<TipType>> seed);
  ~UnionFind() = default;

  /*! \brief add additional types to the union find structure
   */
  void add(std::vector<std::shared_ptr<TipType>> seed);

  std::shared_ptr<TipType> find(std::shared_ptr<TipType> t1);
  void quick_union(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2);
  bool connected(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2);

  friend std::ostream &operator<<(std::ostream &os, const UnionFind &obj);

private:
  // A mapping from terms to parents.
  std::map<std::shared_ptr<TipType>, std::shared_ptr<TipType>> edges;

  std::shared_ptr<TipType> lookup(std::shared_ptr<TipType> t);

  std::shared_ptr<TipType> get_parent(std::shared_ptr<TipType> t);

  // Returns interred equivalent value or creates new interred value
  std::shared_ptr<TipType> smart_insert(std::shared_ptr<TipType> t);

  // Assert datastructure invariants
  void invariant();

  std::ostream &print(std::ostream &out) const;
};
