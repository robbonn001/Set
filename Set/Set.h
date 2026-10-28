#pragma once
#include "BitField.h"
#include <stdexcept>
#include <utility>
#include <iostream>

class Set
{
    BitField bitfield_;

public:
    Set(size_t power) :bitfield_(power) {}
    Set(const Set& other) : bitfield_(other.bitfield_) {}
    Set(Set&& other) noexcept : bitfield_(std::move(other.bitfield_)) {}
    Set(const BitField& other) : bitfield_(other) {}
    Set(BitField&& other) noexcept : bitfield_(std::move(other)) {}
    ~Set() = default;

    size_t maxPower() const noexcept {
        return bitfield_.getBitCount();
    }
    bool isMember(int elem) const {
        return bitfield_.getBit(elem - 1);
    }

    void insert(int elem) {
        if (elem < 1 || elem > maxPower()) throw std::out_of_range("Elem is out of range!");
        bitfield_.setBit(elem - 1);
    }
    void erase(int elem) {
        if (elem < 1 || elem > maxPower()) throw std::out_of_range("Elem is out of range!");
        bitfield_.resetBit(elem - 1);
    }

    operator BitField() const noexcept {
        return bitfield_;
    }

    Set& operator+=(int elem) {
        insert(elem);
        return (*this);
    }
    Set& operator-=(int elem) {
        erase(elem);
        return (*this);
    }
    Set operator+(int elem) const {
        Set temp(*this);
        return (temp += elem);
    }
    Set operator-(int elem) const {
        Set temp(*this);
        return (temp -= elem);
    }
    Set& operator+=(const Set& other) {
        bitfield_ |= other.bitfield_;
        return (*this);
    }
    Set& operator-=(const Set& other) {
        bitfield_ &= ~other.bitfield_;
        return (*this);
    }
    Set& operator*=(const Set& other) {
        bitfield_ &= other.bitfield_;
        return (*this);
    }
    Set operator+(const Set& other) const {
        Set temp(*this);
        return (temp += other);
    }
    Set operator-(const Set& other) const {
        Set temp(*this);
        return (temp -= other);
    }
    Set operator*(const Set& other) const {
        Set temp(*this);
        return (temp *= other);
    }
    Set operator-() const;
    Set& operator=(const Set& other);
    Set& operator=(Set&& other) noexcept;
};

std::istream& operator>>(std::istream& is, Set& other);
std::ostream& operator<<(std::ostream& os, const Set& other);
