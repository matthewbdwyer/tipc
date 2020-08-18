#include "TipRecord.h"
#include "TipTypeVisitor.h"

TipRecord::TipRecord(std::vector<std::shared_ptr<TipType>> inits, std::vector<std::string> names)
  : TipCons(inits), names(names) { }


std::ostream& TipRecord::print(std::ostream &out) const {
    out << "{";
    bool first = true;
    int i = 0;
    for(auto &init : arguments) {
        if(first) {
            out << names.at(i++) << ":" << *init;
            first = false;
            continue;
        }
        out << "," << names.at(i++) << ":" << *init;
    }
    out << "}";
    return out;
}

// This does not obey the semantics of alpha init values 
bool TipRecord::operator==(const TipType &other) const {
    auto tipRecord = dynamic_cast<const TipRecord *>(&other);
    if(!tipRecord) {
        return false;
    }

    if(arity() != tipRecord->arity()) {
        return false;
    }

    for(int i = 0; i < arity(); i++) {
        if(*(arguments.at(i)) != *(tipRecord->arguments.at(i))) {
            return false;
        }
    }

    return true;
}

bool TipRecord::operator!=(const TipType &other) const {
    return !(*this == other);
}

std::vector<std::shared_ptr<TipType>>& TipRecord::getInits() {
    return arguments;
}


std::vector<std::string> const & TipRecord::getNames() const {
    return names;
}

void TipRecord::accept(TipTypeVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto a : arguments) {
       a->accept(visitor);
    }
  }
  visitor->endVisit(this);
}
