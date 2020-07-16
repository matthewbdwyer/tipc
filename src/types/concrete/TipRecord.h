#pragma once
#include "TipCons.h"


class TipRecord: public TipCons {
public:
    TipRecord() = delete;
    TipRecord(std::vector<std::shared_ptr<TipType>> arguments);


};

