#include "TipCons.h"
#include "UnificationError.h"
#include "Unifier.h"
#include <iostream>
#include <sstream>
#include <vector>

// TODO: use dynamic pointer cast.
Unifier::Unifier(std::vector<TypeConstraint> constrs) : constraints(std::move(constrs)) {
    std::vector<std::shared_ptr<TipType>> TipTypes;
    for(TypeConstraint t : constraints) {
        auto l = t.lhs;
        auto r = t.rhs;
        TipTypes.push_back(l);
        TipTypes.push_back(r);

        if(auto f1 = dynamic_cast<TipCons *>(l.get())) {
            for(auto a : f1->arguments) TipTypes.push_back(a);
        }
        if(auto f2 = dynamic_cast<TipCons *>(r.get())) {
            for(auto a : f2->arguments) TipTypes.push_back(a);
        }
    }

    // Deduplicate TipTypes.
    std::vector<std::shared_ptr<TipType>> unique;
    for(auto t : TipTypes) {
        bool add = true;
        for(auto u : unique) {
            if(*t == *u) add = false;
        }
        if(add) unique.push_back(t);
    }

    unionFind = std::make_shared<UnionFind<std::shared_ptr<TipType>>>(unique);
}

void Unifier::solve() {
    for(TypeConstraint constraint: constraints) {
        unify(constraint.lhs, constraint.rhs);
    }
}

void Unifier::unify(std::shared_ptr<TipType> TipType1, std::shared_ptr<TipType> TipType2) {
    //std::cout << "unifying " << TipType1->toString() << " and " << TipType2->toString() << std::endl;
    auto rep1 = unionFind->find(TipType1);
    auto rep2 = unionFind->find(TipType2);

    if(rep1 == rep2) {
       return;
    }

    if(isTipVar(rep1) && isTipVar(rep2)) {
        unionFind->quick_union(rep1, rep2);
    } else if(isTipVar(rep1) && isProperType(rep2)) {
        unionFind->quick_union(rep1, rep2);
    } else if(isProperType(rep1) && isTipVar(rep2)) {
        unionFind->quick_union(rep2, rep1);
    } else if(isCons(rep1) && isCons(rep2)) {
        auto f1 = std::dynamic_pointer_cast<TipCons>(rep1);
        auto f2 = std::dynamic_pointer_cast<TipCons>(rep2);
        if(!f1->do_match(f2)) {
            throwUnifyException(f1, f2);
        }

        unionFind->quick_union(rep1, rep2);
        for(int i = 0; i < f1->arguments.size(); i++) {
            auto a1 = f1->arguments.at(i);
            auto a2 = f2->arguments.at(i);
            unify(a1, a2);
        }
    } else {
        throwUnifyException(TipType1, TipType2);
    }
}

Unifier::~Unifier() {

}

void Unifier::throwUnifyException(std::shared_ptr<TipType> TipType1, std::shared_ptr<TipType> TipType2) {
    std::stringstream s;
    s << "Cannot unify " << TipType1 << "and " << TipType2 <<
        "(respective roots are: " << unionFind->find(TipType1) << " and " <<
        unionFind->find(TipType2) << ")";
    throw UnificationError(s.str().c_str());
}

bool Unifier::isTipVar(std::shared_ptr<TipType> TipType) {
     return std::dynamic_pointer_cast<TipVar>(TipType) != nullptr;
}

bool Unifier::isProperType(std::shared_ptr<TipType> TipType) {
    return std::dynamic_pointer_cast<TipVar>(TipType) == nullptr;
}

bool Unifier::isCons(std::shared_ptr<TipType> TipType) {
    return std::dynamic_pointer_cast<TipCons>(TipType) != nullptr;
}
