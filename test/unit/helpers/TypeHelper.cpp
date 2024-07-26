#include "TypeHelper.h"

#include "TipAlpha.h"
#include "TipFunction.h"
#include "TipInt.h"
#include "TipMu.h"
#include "TipRef.h"

std::shared_ptr<TipType> TypeHelper::intType() {
   return std::make_shared<TipInt>();
}

std::shared_ptr<TipType> TypeHelper::ptrType(std::shared_ptr<TipType> t) {
    return std::make_shared<TipRef>(t);
}

std::shared_ptr<TipType> TypeHelper::funType(std::vector<std::shared_ptr<TipType>> p, std::shared_ptr<TipType> r) {
    return std::make_shared<TipFunction>(p,r);
}
