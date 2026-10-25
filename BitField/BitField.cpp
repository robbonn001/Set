#include "BitField.h"

BitField::BitField(const BitField& other) : bitCount_(other.bitCount_), data_(new BitFieldType[(other.bitCount_ + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK]) {
    for (size_t i = 0;i < getDataSize();++i) data_[i] = other.data_[i];
}

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

BitField& BitField::operator=(const BitField& other) {
    if (this != &other) {
        BitField temp(other);
        swap(*this, temp);
    }
    return (*this);
}

BitField& BitField::operator=(BitField&& other) noexcept {
    if (this != &other) {
        bitCount_ = other.bitCount_;
        data_ = other.data_;
        other.data_ = nullptr;
        other.bitCount_ = 0;
    }
    return (*this);
}

BitField& BitField::operator|=(const BitField& other) {
    if (this != &other) {
        if (getBitCount() != other.getBitCount()) throw std::logic_error("Different powers");
        for (size_t i = 0;i < getDataSize();i++) {
            data_[i] |= other.data_[i];
        }
    }
    return (*this);
}

BitField& BitField::operator&=(const BitField& other) {
    if (this != &other) {
        if (getBitCount() != other.getBitCount()) throw std::logic_error("Different powers");
        for (size_t i = 0;i < getDataSize();i++) {
            data_[i] &= other.data_[i];
        }
    }
    return (*this);
}

BitField BitField::operator|(const BitField& other) const {
    if (getBitCount() != other.getBitCount()) throw std::logic_error("Different powers");
    BitField temp(*this);
    return temp |= other;
}

BitField BitField::operator&(const BitField& other) const {
    if (getBitCount() != other.getBitCount()) throw std::logic_error("Different powers");
    BitField temp(*this);
    return temp &= other;
}

BitField BitField::operator~() const {
    BitField temp(*this);
    for (size_t i = 0;i < getDataSize();i++) {
        temp.data_[i] = ~temp.data_[i];
    }
    return temp;
}
