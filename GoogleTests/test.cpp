#include "pch.h"
#include "BitField.h"
#include <iostream>
#include <stdexcept>

TEST(TestBitField, TestConstructorZeroPower) {
    BitField bitfield;
    EXPECT_EQ(bitfield.getBitCount(), 0);
    EXPECT_EQ(bitfield.getDataSize(), 0);
}

TEST(TestBitField, TestConstructorOnePower) {
    BitField bitfield(1);
    EXPECT_EQ(bitfield.getBitCount(), 1);
    EXPECT_EQ(bitfield.getDataSize(), 1);
}

TEST(TestBitField, TestConstructorSomePower) {
    size_t power = 100;
    BitField bitfield(power);
    EXPECT_EQ(bitfield.getBitCount(), power);
    EXPECT_EQ(bitfield.getDataSize(), (power + BitField::BITS_PER_BLOCK - 1) / BitField::BITS_PER_BLOCK);
}

TEST(TestBitField, TestSetAndResetBit) {
    BitField bitfield(1);
    bitfield.setBit(0);
    EXPECT_EQ(bitfield.getBit(0),1);
    bitfield.resetBit(0);
    EXPECT_EQ(bitfield.getBit(0), 0);
}

TEST(TestBitField, TestSetAndResetBitException) {
    BitField bitfield(5);
    EXPECT_THROW(bitfield.setBit(5), std::out_of_range);
    EXPECT_THROW(bitfield.resetBit(5), std::out_of_range);
    EXPECT_THROW(bitfield.getBit(5), std::out_of_range);
}

// Тест на граничные значения (стыки блоков и конец массива)
TEST(TestBitField, TestBoundaryBits) {
    BitField bitfield(64);

    bitfield.setBit(31); // Последний бит первого блока (для 32 бит)
    bitfield.setBit(32); // Первый бит второго блока
    bitfield.setBit(63); // Последний бит вообще

    EXPECT_TRUE(bitfield.getBit(31));
    EXPECT_TRUE(bitfield.getBit(32));
    EXPECT_TRUE(bitfield.getBit(63));

    // Проверяем, что соседние биты не заделись
    EXPECT_FALSE(bitfield.getBit(30));
    EXPECT_FALSE(bitfield.getBit(33));
}

// Тест множественных установок и сбросов
TEST(TestBitField, TestMultipleSetAndReset) {
    BitField bitfield(100);

    bitfield.setBit(5);
    bitfield.setBit(50);
    bitfield.setBit(99);

    EXPECT_TRUE(bitfield.getBit(5));
    EXPECT_TRUE(bitfield.getBit(50));
    EXPECT_TRUE(bitfield.getBit(99));

    bitfield.resetBit(50);

    EXPECT_TRUE(bitfield.getBit(5));
    EXPECT_FALSE(bitfield.getBit(50));
    EXPECT_TRUE(bitfield.getBit(99));
}

// Тест move-конструктора
TEST(TestBitField, TestMoveConstructor) {
    BitField bitfield1(32);
    bitfield1.setBit(10);

    BitField bitfield2(std::move(bitfield1));

    // Новый объект должен содержать данные
    EXPECT_TRUE(bitfield2.getBit(10));
    EXPECT_EQ(bitfield2.getBitCount(), 32);

    // Старый объект должен быть обнулен
    EXPECT_EQ(bitfield1.getBitCount(), 0);
}

// Тест оператора перемещения
TEST(TestBitField, TestMoveAssignment) {
    BitField bitfield1(64);
    bitfield1.setBit(40);

    BitField bitfield2(10);
    bitfield2 = std::move(bitfield1);

    EXPECT_TRUE(bitfield2.getBit(40));
    EXPECT_EQ(bitfield2.getBitCount(), 64);
    EXPECT_EQ(bitfield1.getBitCount(), 0);
}
