#pragma once

#include "memory"

#include "TipType"

class TypeHelper {
public:
    static std::shared_ptr<TipType> TypeHelper::intType();
    static std::shared_ptr<TipType> TypeHelper::ptrType(std::shared_ptr<TipType> t);
    static std::shared_ptr<TipType> TypeHelper::funType(std::vector<std::shared_ptr<TipType>> p, std::shared_ptr<TipType> r);
};
