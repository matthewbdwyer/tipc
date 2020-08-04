#include "Error.h"

const char* Error::what() const noexcept {
    return msg.c_str();
}
