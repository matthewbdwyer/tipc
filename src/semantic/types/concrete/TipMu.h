#pragma once

#include <string>
#include "TipType.h"
#include "Mu.h"
#include "Var.h"
#include "Term.h"
#include "../../AST.h"

class TipMu: public TipType, public Mu {
public:
    TipMu() = delete;
    TipMu(std::shared_ptr<Var> v, std::shared_ptr<Term> t);
    std::string toString() override ;
    std::ostream& print(std::ostream &out) const override;
    bool operator==(const Term& other) const override;
    bool operator!=(const Term& other) const override;

    std::shared_ptr<Var> v;
    std::shared_ptr<Term> t;
};


