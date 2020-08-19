#include "UnionFind.h"

#include "loguru.hpp"
#include <assert.h>
#include <iostream>

namespace { // Anonymous namespace for local helpers
bool verbose = false;
}

void UnionFind::print() {
    std::cout << "UnionFind graph:\n";
    for(auto &p : edges) {
      std::cout << "  edges[" << *p.first << "] = " << *p.second << std::endl;
    }
    std::cout << std::endl;
}

UnionFind::UnionFind(std::vector<std::shared_ptr<TipType>> seed) {
    for(auto &term : seed) {
        smart_insert(term);
    }
}

// TODO (nphair): Add path compression - its practically free performance.
std::shared_ptr<TipType> UnionFind::find(std::shared_ptr<TipType> t) {
    LOG_S(1) << "UnionFind looking for representive of " << *t;

    // Effectively a noop if the term is already in the map.
    smart_insert(t);

    auto parent = t;
    while(*get_parent(parent) != *parent) {
        parent = get_parent(parent);
    }

    LOG_S(1) << "UnionFind found representative " << *parent;

    return parent;
}

// TODO (nphair): Weight the subtrees and be smarter about our unioning.
void UnionFind::quick_union(std::shared_ptr<TipType> t1, std::shared_ptr<TipType> t2) {
    smart_insert(t1);
    smart_insert(t2);

    auto t1_root = find(t1);
    auto t2_root = find(t2);

    // semantics-based insert
    for(auto const &edge : edges) {
        if(*t1_root == *edge.first) {
            edges.erase(edge.first);
            edges.insert(std::pair<std::shared_ptr<TipType>, std::shared_ptr<TipType>>(t1_root, t2_root));
            break;
        }
    }

    LOG_S(1) << "UnionFind union " << *t1 << " and " << *t2;
    LOG_S(1) << " by setting edges[" << *t1_root << "] to " << *t2_root;
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
    
    LOG_S(1) << "UnionFind inserting term " << *t;

    for(auto const &edge : edges) {
        if(*t == *edge.first) {
            LOG_S(1) << " ; already in the graph as " << *edge.first;
            return;
        }
    }

    LOG_S(1) << " ; adding new edge\n";
    edges.insert(std::pair<std::shared_ptr<TipType>, std::shared_ptr<TipType>>(t, t));
}

