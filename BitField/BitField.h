#pragma once
#include <stdexcept>

class BitField {

public:
    typedef uint32_t BitFieldType;
    static constexpr size_t BITS_PER_BLOCK = sizeof(BitFieldType) * 8;
    BitField(BitFieldType power = 0) : bitCount(power), data_(new BitFieldType[(bitCount + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK]()) {}
    BitField(const BitField& other) = delete;
    BitField(BitField&& other) noexcept : bitCount(other.bitCount), data_(other.data_) {
        other.data_ = nullptr;
        other.bitCount = 0;
    }
    ~BitField() {
        if (data_) delete[] data_;
    }
    BitFieldType getBitCount() const noexcept { return bitCount; }
    size_t getDataSize() const noexcept { return (bitCount + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK; }

    void setBit(size_t pos);
    void resetBit(size_t pos);
    bool getBit(size_t pos) const;
    BitField& operator=(const BitField& other) = delete;
    BitField& operator=(BitField&& other) noexcept;

private:
    BitFieldType bitCount;
    BitFieldType* data_;
    size_t getIndex(size_t pos) const {
        if (pos >= bitCount) throw std::out_of_range("Pos is out of range");
        return pos >> sizeof(BitFieldType);
    }
    BitFieldType getMask(size_t pos) const noexcept {
        return static_cast<BitFieldType>(1) << (pos % BITS_PER_BLOCK);
    }
};
