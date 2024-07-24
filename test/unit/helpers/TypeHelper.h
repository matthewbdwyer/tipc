#pragma once

#include "memory"
#include <vector>

#include <TipType.h>

class TypeHelper {
public:
    static std::shared_ptr<TipType> intType();
    static std::shared_ptr<TipType> ptrType(std::shared_ptr<TipType> t);
    static std::shared_ptr<TipType> funType(std::vector<std::shared_ptr<TipType>> p, std::shared_ptr<TipType> r);
};
