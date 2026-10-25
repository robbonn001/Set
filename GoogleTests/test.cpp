#include "pch.h"
#include "BitField.h"
#include <iostream>
#include <stdexcept>
#include <utility>

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

TEST(TestBitField, TestCopyConstructor) {
    size_t power = 100;
    BitField first(power);
    BitField second(first);
    first.setBit(1);
    EXPECT_TRUE(first.getBit(1));
    EXPECT_FALSE(second.getBit(1));
}

TEST(TestBitField, TestSetAndResetBit) {
    BitField bitfield(1);
    bitfield.setBit(0);
    EXPECT_TRUE(bitfield.getBit(0));
    bitfield.resetBit(0);
    EXPECT_FALSE(bitfield.getBit(0));
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

// Тест оператора копирования
TEST(TestBitField, TestCopyAssignment) {
    size_t power = 100;
    BitField first(power);
    BitField second(power/2);
    EXPECT_EQ(second.getBitCount(), 50);
    second = first;
    EXPECT_EQ(second.getBitCount(), 100);

    first.setBit(1);
    EXPECT_TRUE(first.getBit(1));
    EXPECT_FALSE(second.getBit(1));
}

TEST(TestBitField, TestSelfAssignment) {
    BitField bitfield(32);
    bitfield.setBit(10);

    bitfield = bitfield; // самоприсваивание не должно ломать объект

    EXPECT_TRUE(bitfield.getBit(10));
    EXPECT_EQ(bitfield.getBitCount(), 32);
}

TEST(TestBitField, TestOrOperator) {
    BitField a(32), b(32);
    a.setBit(0);
    a.setBit(5);
    b.setBit(5);
    b.setBit(10);

    BitField c = a | b;

    EXPECT_TRUE(c.getBit(0));
    EXPECT_TRUE(c.getBit(5));
    EXPECT_TRUE(c.getBit(10));
    EXPECT_FALSE(c.getBit(1));

    // Исходные объекты не изменились
    EXPECT_FALSE(a.getBit(10));
    EXPECT_FALSE(b.getBit(0));
}

TEST(TestBitField, TestAndOperator) {
    BitField a(32), b(32);
    a.setBit(0);
    a.setBit(5);
    b.setBit(5);
    b.setBit(10);

    BitField c = a & b;

    EXPECT_FALSE(c.getBit(0));
    EXPECT_TRUE(c.getBit(5));   // только общий бит
    EXPECT_FALSE(c.getBit(10));
}

TEST(TestBitField, TestOrAssignmentOperator) {
    BitField a(32), b(32);
    a.setBit(0);
    b.setBit(10);

    BitField& ref = (a |= b); // проверка возврата ссылки

    EXPECT_TRUE(a.getBit(0));
    EXPECT_TRUE(a.getBit(10));
    EXPECT_EQ(&ref, &a);      // оператор возвращает ссылку на себя
}

TEST(TestBitField, TestAndAssignmentOperator) {
    BitField a(32), b(32);
    a.setBit(0);
    a.setBit(5);
    b.setBit(5);
    b.setBit(10);

    a &= b;

    EXPECT_FALSE(a.getBit(0));
    EXPECT_TRUE(a.getBit(5));
    EXPECT_FALSE(a.getBit(10));
}

TEST(TestBitField, TestInvertOperator) {
    BitField a(32);
    a.setBit(0);
    a.setBit(15);

    BitField b = ~a;

    EXPECT_FALSE(b.getBit(0));
    EXPECT_FALSE(b.getBit(15));
    EXPECT_TRUE(b.getBit(1));
    EXPECT_TRUE(b.getBit(16));
    EXPECT_TRUE(b.getBit(31));

    // Исходный объект не изменился
    EXPECT_TRUE(a.getBit(0));
}

TEST(TestBitField, TestDoubleInvert) {
    BitField a(64);
    a.setBit(10);
    a.setBit(50);

    BitField b = ~~a;

    EXPECT_TRUE(b.getBit(10));
    EXPECT_TRUE(b.getBit(50));
    EXPECT_FALSE(b.getBit(0));
}

TEST(TestBitField, TestOperatorsDifferentSize) {
    BitField a(32), b(64);

    EXPECT_THROW(a | b, std::logic_error);   // или std::invalid_argument
    EXPECT_THROW(a & b, std::logic_error);
    EXPECT_THROW(a |= b, std::logic_error);
    EXPECT_THROW(a &= b, std::logic_error);
}

TEST(TestBitField, TestOperatorsBoundaryBits) {
    BitField a(64), b(64);
    a.setBit(31);  // последний бит первого блока
    a.setBit(32);  // первый бит второго блока
    b.setBit(32);
    b.setBit(63);  // последний бит вообще

    BitField c = a | b;
    EXPECT_TRUE(c.getBit(31));
    EXPECT_TRUE(c.getBit(32));
    EXPECT_TRUE(c.getBit(63));
}
