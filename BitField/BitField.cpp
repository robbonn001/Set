#include "BitField.h"

void BitField::setBit(size_t pos) {
    size_t index = getIndex(pos);
    BitFieldType mask = getMask(pos);
    data_[index] |= mask;
}

void BitField::resetBit(size_t pos) {
    size_t index = getIndex(pos);
    BitFieldType mask = getMask(pos);
    data_[index] &= ~mask;
}

bool BitField::getBit(size_t pos) const {
    size_t index = getIndex(pos);
    BitFieldType mask = getMask(pos);
    return (data_[index] & mask) != 0;
}

BitField& BitField::operator=(BitField&& other) noexcept {
    if (this != &other) {
        bitCount = other.bitCount;
        data_ = other.data_;
        other.data_ = nullptr;
        other.bitCount = 0;
    }
    return (*this);
}