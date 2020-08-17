#include "UnionFind.h"
#include <iostream>
#include <assert.h>

namespace { // Anonymous namespace for local helpers
bool verbose = false;
}

UnionFind::UnionFind(std::vector<std::shared_ptr<TipType>> seed) {
    for(auto &term : seed) {
        smart_insert(term);
    }
}

// TODO (nphair): Add path compression - its practically free performance.
std::shared_ptr<TipType> UnionFind::find(std::shared_ptr<TipType> t) {
    if (verbose) {
      std::cout << "UnionFind looking for representive of " << *t << std::endl;
    }

    // Effectively a noop if the term is already in the map.
    smart_insert(t);

    auto parent = t;
    while(*get_parent(parent) != *parent) {
        parent = get_parent(parent);
    }

    if (verbose) {
      std::cout << "UnionFind found representative " << *parent << std::endl;
    }

    return parent;
}

// TODO (nphair): Weight the subtrees and be smarter about our unioning.
void UnionFind::quick_union(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2) {
    auto t1_root = find(t1);
    auto t2_root = find(t2);
    edges[t1_root] = t2_root;
}

bool UnionFind::connected(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2) {
    return *find(t1) == *find(t2);
}

std::shared_ptr<TipType> UnionFind::get_parent(std::shared_ptr<TipType> t) {
    for(auto const &edge : edges) {
        if(*t == *edge.first) {
            return edge.second;
        }
    }

    assert(0);
}

/**
 * Inserts should be based on the dereferenced value.
 */
void UnionFind::smart_insert(std::shared_ptr<TipType> t) {
    if(t == nullptr) {
        throw std::invalid_argument("Refusing to insert a nullptr into the map.");
    }
    
    if (verbose) {
      std::cout << "UnionFind inserting term " << *t;
    }

    for(auto const &edge : edges) {
        if(*t == *edge.first) {
            if (verbose) {
              std::cout << " ; already in the graph as " << *edge.first << std::endl; 
            } 
            return;
        }
    }
    if (verbose) {
      std::cout << " ; adding new edge\n"; 
    }
    edges.insert(std::pair<std::shared_ptr<TipType>, std::shared_ptr<TipType>>(t, t));
}
