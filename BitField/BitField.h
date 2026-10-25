#pragma once
#include <stdexcept>

class BitField {

public:
    typedef uint32_t BitFieldType;
    static constexpr size_t BITS_PER_BLOCK = sizeof(BitFieldType) * 8;
    BitField(BitFieldType power = 0) : bitCount_(power), data_(new BitFieldType[(bitCount_ + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK]()) {}
    BitField(const BitField& other);
    BitField(BitField&& other) noexcept : bitCount_(other.bitCount_), data_(other.data_) {
        other.data_ = nullptr;
        other.bitCount_ = 0;
    }
    ~BitField() {
        if (data_) {
            delete[] data_;
        }
    }
    BitFieldType getBitCount() const noexcept { return bitCount_; }
    size_t getDataSize() const noexcept { return (bitCount_ + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK; }

    void setBit(size_t pos);
    void resetBit(size_t pos);
    bool getBit(size_t pos) const;

    BitField& operator=(const BitField& other);
    BitField& operator=(BitField&& other) noexcept;

    BitField& operator|=(const BitField& other);
    BitField& operator&=(const BitField& other);
    BitField operator|(const BitField& other) const;
    BitField operator&(const BitField& other) const;
    BitField operator~() const;

    friend void swap(BitField& first, BitField& second) {
        using std::swap;
        swap(first.bitCount_, second.bitCount_);
        swap(first.data_, second.data_);
    }

private:
    BitFieldType bitCount_;
    BitFieldType* data_;
    size_t getIndex(size_t pos) const {
        if (pos >= bitCount_) throw std::out_of_range("Pos is out of range");
        return pos / BITS_PER_BLOCK;
    }
    BitFieldType getMask(size_t pos) const noexcept {
        return static_cast<BitFieldType>(1) << (pos % BITS_PER_BLOCK);
    }
};
