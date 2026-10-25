#include "Set.h"

Set Set::operator-() const {
    Set temp(*this);
    temp.bitfield_ = ~temp.bitfield_;
    return temp;
}

Set& Set::operator=(const Set& other) {
    if (this != &other) {
        Set temp(other);
        swap(*this, temp);
    }
    return (*this);
}

Set& Set::operator=(Set&& other) noexcept {
    if (this != &other) {
        bitfield_ = std::move(other.bitfield_);
    }
    return (*this);
}
