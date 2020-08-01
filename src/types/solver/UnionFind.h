#pragma once

#include <map>
#include <vector>

template <class T>
class UnionFind {
public:
    UnionFind() = delete ;
    UnionFind(std::vector<T *> seed);

    T * find(T * t);
    void quick_union(T * t1, T * t2);
    bool connected(T * t1, T * t2);
    void print_edges();
    std::map<T *, T *>& get_edges();
private:
    // A mapping from terms to parents.
    T * get_parent(T *);
    std::map<T *, T *> edges;

};

#include "UnionFind.tpp"
