#include <iostream>
#include <assert.h>


template<class T>
UnionFind<T>::UnionFind(std::vector<T> seed) {
    for(auto term : seed) {
        edges.insert(std::pair<T, T>(term, term));
    }
}

template<class T>
T UnionFind<T>::find(T t) {
    // TODO (nphair): Add path compression - its practically free performance.
    if(!get_parent(t)) {
        assert(0);
    }

    T parent = t;
    while(get_parent(parent) != parent) {
        parent = get_parent(parent);
    }

    // HACK (nphair): Wrap parent in a get_parent call so we don't introduce a term that
    // is equal but not identical.
    return get_parent(parent);
}

template<class T>
void UnionFind<T>::quick_union(T t1, T t2) {
    // TODO (nphair): Weight the subtrees and be smarter about our unioning.
    T t1_root = find(t1);
    T t2_root = find(t2);
    // if its not a ptr then what...
    //if(t1_root == nullptr || t2_root == nullptr) {
    //    std::cerr << "can't union elements not in structure" << std::endl;
    //    assert(0);
    //}

    edges[t1_root] = t2_root;
}

template<class T>
bool UnionFind<T>::connected(T t1, T t2) {
    return find(t1) == find(t2);
}

template<class T>
T UnionFind<T>::get_parent(T t) {
    for(auto edge : edges) {
        if(edge.first == t) return edge.second;
    }
    assert(0);
}

template<class T>
void UnionFind<T>::print_edges() {
    for(auto e : edges) {
        std::cout << e.first->toString() << " --> " << find(e.first)->toString() << ")" << std::endl;
    }
}

