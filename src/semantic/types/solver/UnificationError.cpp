#include "UnificationError.h"

UnificationError::UnificationError(const char *msg) {
    this->msg = msg;
}

const char * UnificationError::what() const throw() {
    return msg;
}
