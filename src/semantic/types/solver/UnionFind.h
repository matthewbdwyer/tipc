#pragma once

#include <map>
#include <vector>
#include <TipType.h>

/*!
 * \class UnionFind
 *
 * \brief Specialized implementation of a union-find data structure tailored to work with
 * TipTypes wrapped in shared pointers.
 */
class UnionFind {
public:
    UnionFind() = default;
    explicit UnionFind(std::vector<std::shared_ptr<TipType>> seed);
    ~UnionFind() = default;

    std::shared_ptr<TipType> find(std::shared_ptr<TipType> t1);
    void quick_union(std::shared_ptr<TipType> t1, std::shared_ptr<TipType>t2);
    bool connected(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2);

    /*! \brief Returns a deep copy of the union-find structure.
     */
    std::unique_ptr<UnionFind> copy();

private:
    // A mapping from terms to parents.
    std::map<std::shared_ptr<TipType>, std::shared_ptr<TipType>> edges;

    std::shared_ptr<TipType> get_parent(std::shared_ptr<TipType> t);
    void smart_insert(std::shared_ptr<TipType> t);
    std::ostream& print(std::ostream &out) const;
};

