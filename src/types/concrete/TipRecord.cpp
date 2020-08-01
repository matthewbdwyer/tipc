#include "TipRecord.h"

TipRecord::TipRecord(std::vector<std::shared_ptr<TipType>> inits,
                     std::vector<std::string> names) : 
       inits(inits), names(names) { }


int TipRecord::arity() {
    return inits.size();
}

std::ostream& TipRecord::print(std::ostream &out) const {
    out << "{";
    bool first = true;
    int i = 0;
    for(auto &init : inits) {
        if(first) {
            out << names.at(i++) << ":" << init.get();
            first = false;
            continue;
        }
        out << ", " << names.at(i++) << ":" << init.get();
    }
    out << "}";
    return out;
}

// This does not obey the semantics of alpha init values 
bool TipRecord::operator==(const TipType &other) const {
    if(auto tipRecord = dynamic_cast<const TipRecord *>(&other)) {
        if(inits.size() != tipRecord->inits.size()) {
            return false;
        }

        for(int i = 0; i < inits.size(); i++) {
            if(*(inits.at(i)) != *(tipRecord->inits.at(i))) {
                return false;
            }
        }

        return true;
    }
    return false;
}

bool TipRecord::operator!=(const TipType &other) const {
    return !(*this == other);
}

