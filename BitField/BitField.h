#pragma once
#include <stdexcept>

class BitField
{
    typedef unsigned int BitFieldType;
    BitFieldType size_;
    BitFieldType* data_;

public:
    BitField(BitFieldType size) : size_(size), data_(new BitFieldType[size_ / sizeof(BitFieldType) + 1]) {}
    ~BitField() {
        if (data_) delete[] data_;
    }
    BitFieldType getSize() const noexcept { return size_; }
    BitFieldType getMaxPower() const noexcept { return size_ / sizeof(BitFieldType) + 1; }

    void setBit(size_t pos);
    void resetBit(size_t pos);
    bool getBit(size_t pos) const;

private:
    size_t getIndex(size_t pos) const {
        if (pos >= size_) throw std::logic_error("Pos is out of range");
        return pos >> sizeof(BitFieldType);
    }
    BitFieldType getMask(size_t pos) const noexcept {
        return 1 << (pos % sizeof(BitFieldType));
    }
};
