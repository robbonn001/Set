#include "Set.h"

Set Set::operator-() const {
    Set temp(*this);
    temp.bitfield_ = ~temp.bitfield_;
    return temp;
}

Set& Set::operator=(const Set& other) {
    if (this != &other) {
        bitfield_ = other.bitfield_;
    }
    return (*this);
}

Set& Set::operator=(Set&& other) noexcept {
    if (this != &other) {
        bitfield_ = std::move(other.bitfield_);
    }
    return (*this);
}

std::istream& operator>>(std::istream& is, Set& other) {
    char symbol;
    int num = 0;
    while (is >> symbol && symbol != '\n' && symbol != '}') {
        if (symbol == ' ' || symbol == '{') continue;
        if (symbol != ',') {
            num += (symbol - '0');
            num *= 10;
        }
        else {
            other.insert(num / 10);
            num = 0;
        }
    }
    other.insert(num / 10);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Set& other) {
    os << "{ ";
    bool flag = false;
    for (size_t i = 1;i <= other.maxPower();++i) {
        if (other.isMember(i)) {
            if (flag) os << ", ";
            os << i;
            flag = true;
        }
    }
    os << " }\n";
    return os;
}
