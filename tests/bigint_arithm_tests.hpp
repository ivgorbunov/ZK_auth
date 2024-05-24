#pragma once

#include "bigint.hpp"
#include "bigint_test_helper.hpp"

TEST(BigIntOperatorTests, PlusRandom) {
    TEST_SAME_OPERATOR(+, +=);
}

TEST(BigIntOperatorTests, PlusMemory) {
    CHECK_OPERATOR_ALLOCATIONS(+, 1)
}

TEST(BigIntOperatorTests, MinusRandom) {
    TEST_SAME_OPERATOR(-, -=);
}

TEST(BigIntOperatorTests, MinusMemory) {
    CHECK_OPERATOR_ALLOCATIONS(-, 1);
}

TEST(BigIntOperatorTests, MultRandom) {
    TEST_SAME_OPERATOR(*, *=);
}

TEST(BigIntOperatorTests, MultMemory) {
    CHECK_OPERATOR_ALLOCATIONS(*, 3);
}

TEST(BigIntOperatorTests, DivRandom) {
    TEST_SAME_OPERATOR(/, /=);
}

TEST(BigIntOperatorTests, DivMultRandom) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        BigInteger first = random_bigint(100);
        BigInteger second = random_bigint(50);
        auto [quot, rem] = BigInteger::divide(first, second);
        ASSERT_TRUE(BigInteger(0) <= rem);
        ASSERT_TRUE(rem < second);
        ASSERT_EQ(first, quot * second + rem);
    }
}

TEST(BigIntOperatorTests, DivMemory) {
    CHECK_OPERATOR_ALLOCATIONS(/, 2);
}

TEST(BigIntOperatorTests, ModRandom) {
    TEST_SAME_OPERATOR(%, %=);
}

TEST(BigIntOperatorTests, ModMemory) {
    CHECK_OPERATOR_ALLOCATIONS(%, 4);
}


TEST(BigIntOperatorTests, PlusEQ) {
    BigInteger a = 179;
    a += 179;
    
    ASSERT_EQ(179+179, a);
}

TEST(BigIntOperatorTests, PlusEQNeg) {
    BigInteger a = -179;
    a += 57;

    ASSERT_EQ(57-179, a);
}

TEST(BigIntOperatorTests, PlusEQNegNeg) {
    BigInteger a = -179;
    a += -57;

    ASSERT_EQ(-179-57, a);
}

TEST(BigIntOperatorTests, PlusEQNegPos) {
    BigInteger a = -57;
    a += 179;

    ASSERT_EQ(179-57, a);
}

TEST(BigIntOperatorTests, PlusEQPosNeg) {
    BigInteger a = 179;
    a += -57;
    ASSERT_EQ(179-57, a);
}

TEST(BigIntOperatorTests, PlusEQPosNegOverflow) {
    BigInteger a = 57;
    a += -179;

    ASSERT_EQ(57-179, a);
}

TEST(BigIntOperatorTests, PlusEQRandom) {
    for (auto i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        long long first = random_value();
        long long second = random_value();
        BigInteger a = first;
        a += second;
        ASSERT_EQ(first + second, a);
    }
}

TEST(BigIntOperatorTests, PlusEQTime) {
    int total_time = 0;
    int time_treshold = 1000000;
    for (int i = 0; i < 2000; ++i) {
        BigInteger first = random_bigint(1e4);
        BigInteger second = random_bigint(1e4);
        Timer T;
        T.start();
        first += second;
        T.finish();
        total_time += T.get_time_microseconds();
        ASSERT_LE(total_time, time_treshold) << i << " iteration of 1000";
    }
}

TEST(BigIntOperatorTests, PlusEqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(+=, 1);
}

TEST(BigIntOperatorTests, MinusEQ) {
    BigInteger a = 11;
    for (int i = -25; i < 25; ++i) {
        a = 11;
        a -= i;
        ASSERT_EQ(11 - i, a);
    }
}

TEST(BigIntOperatorTests, MinusEQRandom) {
    for (auto i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        long long first = random_value();
        long long second = random_value();
        BigInteger a = first;
        a -= second;
        ASSERT_EQ(first - second, a);
    }
}

TEST(BigIntOperatorTests, MinusEqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(-=, 1);
}

void test_mult(long long a, long long b) {
    BigInteger bi = a;
    bi *= b;

    ASSERT_EQ(a * b, bi);
}

TEST(BigIntOperatorTests, TimesEQ) {
    test_mult(57, 4);
}

TEST(BigIntOperatorTests, TimesEQNeg) {
    test_mult(-57, 4);
}

TEST(BigIntOperatorTests, TimesEQNegNeg) {
    test_mult(-57, -4);
}

TEST(BigIntOperatorTests, TimesEQPosNeg) {
    test_mult(57, -4);
}

TEST(BigIntOperatorTests, TimesEQZero) {
    test_mult(1791791791, 0);
}

TEST(BigIntOperatorTests, TimesEQZeroNeg) {
    test_mult(-1791791791, 0);
}

TEST(BigIntOperatorTests, TimesEQRandom) {
    for (auto i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        test_mult(random_value(), random_value());
    }
}

TEST(BigIntOperatorTests, TimesEQTime) {
    int total_time = 0;
    int time_treshold = 3000;

    for (int i = 0; i < 100; ++i) {
        auto first = random_bigint(1e4);
        auto second = random_bigint(1e4);
        Timer T;
        T.start();
        first *= second;
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold) << i << " of 100 iterations";
    }
}

TEST(BigIntOperatorTests, TimesEqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(*=, 2);
}

void test_division(long long left, long long right) {
    BigInteger a = left;
    a /= right;

    ASSERT_EQ(left / right, a);
}

TEST(BigIntOperatorTests, DivEQ) {
    test_division(179, 57);
}

TEST(BigIntOperatorTests, DivNeg) {
    test_division(-179, 57);
}

TEST(BigIntOperatorTests, DivPosNeg) {
    test_division(-179, 57);
}

TEST(BigIntOperatorTests, DivNegNeg) {
    test_division(-179, -57);
}

TEST(BigIntOperatorTests, DivEqRandom) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        auto left = random_value();
        auto right = 0;
        while (right == 0) right = random_value();
        test_division(left, right);
    }
}

TEST(BigIntOperatorTests, DivTime) {
    int total_time = 0;
    int time_treshold = 2000;

    for (int i = 0; i < 100; ++i) {
        auto first = random_bigint(5e2);
        auto second = random_bigint(5e2);
        Timer T;
        T.start();
        first /= second;
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(BigIntOperatorTests, DivEqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(/=, 1);
}

void test_modulus(long long left, long long right) {
    BigInteger a = left;
    a %= right;
    
    ASSERT_EQ(left % right, a);
}

TEST(BigIntOperatorTests, Mod) {
    test_modulus(179, 57);
}

TEST(BigIntOperatorTests, ModNeg) {
    test_modulus(-179, 57);
}

TEST(BigIntOperatorTests, ModPosNeg) {
    test_modulus(179, -57);
}

TEST(BigIntOperatorTests, ModNegNeg) {
    test_modulus(-179, -179);
}

TEST(BigIntOperatorTests, ModEqRandom) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
        long long left = random_value();
        long long right = 0;
        while (right == 0) right = random_value();

        test_modulus(left, right);
    }
}

TEST(BigIntOperatorTests, ModEqTime) {
    int total_time = 0;
    int time_treshold = 2000;

    for (int i = 0; i < 100; ++i) {
        auto first = random_bigint(5e2);
        auto second = random_bigint(5e2);
        Timer T;
        T.start();
        first %= second;
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}

TEST(BigIntOperatorTests, ModEqMemory) {
    CHECK_OPERATOR_ALLOCATIONS(%=, 3);
}

TEST(BigIntOperatorTests, LPPlus) {
    BigInteger a = 178;
    ASSERT_EQ(179, ++a);
    ASSERT_EQ(179, a);
}

TEST(BigIntOperatorTests, LPPlusNeg) {
    BigInteger a = -180;
    ASSERT_EQ(-179, ++a);
    ASSERT_EQ(-179, a);
}

TEST(BigIntOperatorTests, LPPlusNegZero) {
    BigInteger a = -1;
    ASSERT_EQ(0, ++a);
    ASSERT_EQ(0, a);
}

TEST(BigIntOperatorTests, LPPlusDoublePlus) {
    BigInteger a = 177;
    ASSERT_EQ(179, ++++a);
    ASSERT_EQ(179, a);
}

TEST(BigIntOperatorTests, LPPlusTime) {
    int total_time = 0;
    int time_treshold = 1500000;
    
    for (int i = 0; i < 10000; ++i) {
        Timer T;
        BigInteger first = random_bigint(9);
        T.start();
        
        for (int j = 0; j < 1e3; ++j) {
            ++first;
        }

        T.finish();
        total_time += T.get_time_microseconds();
        ASSERT_LE(total_time, time_treshold) << i << " of 10000 iterations";
    }
}

TEST(BigIntOperatorTests, LPPlusMemory) {
    BigInteger a = 179;
    OperatorNewCounter cntr;
    ++a;
    ASSERT_EQ(0, cntr.get_counter());
}

TEST(BigIntOperatorTests, RPPlus) {
    BigInteger a = 178;
    ASSERT_EQ(178, a++);
    ASSERT_EQ(179, a);
}

TEST(BigIntOperatorTests, RPPlusNeg) {
    BigInteger a = -180;
    ASSERT_EQ(-180, a++);
    ASSERT_EQ(-179, a);
}

TEST(BigIntOperatorTests, RPPlusNegZero) {
    BigInteger a = -1;
    ASSERT_EQ(-1, a++);
    ASSERT_EQ(0, a);
}

TEST(BigIntOperatorTests, RPPlusMemory) {
    BigInteger a = 179;
    OperatorNewCounter cntr;
    BigInteger b = a++;
    ASSERT_LE(cntr.get_counter(), 1);
}

TEST(BigIntOperatorTests, LMMinus) {
    BigInteger a = 180;
    ASSERT_EQ(179, --a);
    ASSERT_EQ(179, a);
}

TEST(BigIntOperatorTests, LMMinusNeg) {
    BigInteger a = -178;
    ASSERT_EQ(-179, --a);
    ASSERT_EQ(-179, a);
}

TEST(BigIntOperatorTests, LMMinusPosZero) {
    BigInteger a = 1;
    ASSERT_EQ(0, --a);
    ASSERT_EQ(0, a);
}

TEST(BigIntOperatorTests, LMMinusDoubleMinus) {
    BigInteger a = 181;
    ASSERT_EQ(179, ----a);
    ASSERT_EQ(179, a);
}

TEST(BigIntOperatorTests, LMMinusTime) {
    int total_time = 0;
    int time_treshold = 1500;
    
    for (int i = 0; i < 100; ++i) {
        Timer T;
        BigInteger first = random_bigint(1e2);
        T.start();
        
        for (int j = 0; j < 1e5; ++j) {
            --first;
        }

        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold) << i << " of 100 iterations";
    }
}

TEST(BigIntOperatorTests, LMMinusMemory) {
    BigInteger a = 180;
    OperatorNewCounter cntr;
    --a;
    ASSERT_EQ(cntr.get_counter(), 0);
}

TEST(BigIntOperatorTests, RMMinus) {
    BigInteger a = 180;
    ASSERT_EQ(180, a--);
    ASSERT_EQ(179, a);
}

TEST(BigIntOperatorTests, RMMinusNeg) {
    BigInteger a = -178;
    ASSERT_EQ(-178, a--);
    ASSERT_EQ(-179, a);
}

TEST(BigIntOperatorTests, RMMinusPosZero) {
    BigInteger a = 1;
    ASSERT_EQ(1, a--);
    ASSERT_EQ(0, a);
}

TEST(BigIntOperatorTests, RMMinusMemory) {
    BigInteger a = 180;
    OperatorNewCounter cntr;
    auto b = a--;
    ASSERT_LE(cntr.get_counter(), 1);
}

TEST(BigIntOperatorTests, UnaryMinus) {
    for (int i = -25; i < 25; ++i) {
        BigInteger a(i);
        BigInteger b = -a;
        ASSERT_EQ(b, -i);
    }
    for (int i = 0; i < 10; ++i) {
        BigInteger a = random_bigint(100);
        BigInteger b = -a;
        BigInteger c = -b;
        BigInteger d = -c;
        ASSERT_EQ(a, c);
        ASSERT_EQ(b, d);
        ASSERT_EQ(a + b, 0);
    }
}

/*
TEST(BigIntMethodsTests, Power) {
    BigInteger a = BigInteger::power(2, 6);
    ASSERT_EQ(a, 64);
}

TEST(BigIntMethodsTests, PowerZero) {
    BigInteger a = BigInteger::power(2, 0);
    ASSERT_EQ(1, a);
}

TEST(BigIntMethodTests, PowerBig) {
    BigInteger a = BigInteger::power(2, 16);
    ASSERT_EQ(65536, a);
}

TEST(BigIntMethodsTests, PowerOne) {
    BigInteger a = BigInteger::power(1, 1791791791);
    ASSERT_EQ(1, a);
}

TEST(BigIntMethodsTests, NegPower) {
    BigInteger a = BigInteger::power(-2, 5);
    ASSERT_EQ(-32, a);
}

TEST(BigIntMethodsTests, NegEvenPower) {
    BigInteger a = BigInteger::power(-2, 8);
    ASSERT_EQ(256, a);
}

TEST(BigIntMethodsTests, ZeroPower) {
    BigInteger a = BigInteger::power(0, 1791791791);
    ASSERT_EQ(0, a);
}

TEST(BigIntMethodsTests, PowerTime) {
    int time_treshold = 2500;
    int total_time = 0;

    for (size_t i = 0; i < 100; ++i) {
        Timer T;
        T.start();
        BigInteger::power(random_bigint(3), random_bigint(3));
        T.finish();
        total_time += T.get_time_milliseconds();

        ASSERT_LE(total_time, time_treshold) << i << " iteration of 100";
    }
}

TEST(BigIntMethodsTests, Size) {
    BigInteger a = 179;

    ASSERT_EQ(3, a.size());
}

TEST(BigIntMethodsTests, SizeNegative) {
    BigInteger a = -19;
    ASSERT_EQ(2, a.size());
}

TEST(BigIntMethodsTests, SizeBig) {
    BigInteger a = random_bigint(100'000);
    ASSERT_EQ(100'000, a.size());
}

TEST(BigIntMethodsTests, SizeZero) {
    BigInteger a = 0;
    ASSERT_EQ(1, a.size());
}*/

TEST(BigIntMethodsTests, IsZeroPositive) {
    BigInteger a = 179;
    ASSERT_FALSE(a.is_zero());
}

TEST(BigIntMethodsTests, IsZeroNegative) {
    BigInteger a = -179;
    ASSERT_FALSE(a.is_zero());
}

TEST(BigIntMethodsTests, IsZeroZero) {
    BigInteger a = 0;
    ASSERT_TRUE(a.is_zero());
}

TEST(BigIntMethodsTests, IsZeroTen) {
    BigInteger a = 10;
    ASSERT_FALSE(a.is_zero());
}

TEST(BigIntMethodsTests, IsNegativePositive) {
    BigInteger a = 179;
    ASSERT_FALSE(a.is_negative());
}

TEST(BigIntMethodsTests, IsNegativeZero) {
    BigInteger a = 0;
    ASSERT_FALSE(a.is_negative());
}

TEST(BigIntMethodsTests, IsNegativeNegative) {
    BigInteger a = -1;
    ASSERT_TRUE(a.is_negative());
}

TEST(BigIntMethodsTests, IsNegativeAfterSum) {
    BigInteger a = -11;
    a += 11;
    ASSERT_FALSE(a.is_negative());
}

/*
TEST(BigIntMethodsTests, InvertSignPositive) {
    BigInteger a = 179;
    a.invert_sign();
    ASSERT_EQ(-179, a);
}

TEST(BigIntMethodsTests, InvertSignNegative) {
    BigInteger a = -179;
    a.invert_sign();
    ASSERT_EQ(179, a);
}

TEST(BigIntMethodsTests, InvertSignZero) {
    BigInteger a = 0;
    a.invert_sign();
    ASSERT_EQ(0, a);
    ASSERT_FALSE(a.is_negative());
}

TEST(BigIntMethodsTests, Shift) {
    BigInteger a = 179;
    a.shift(1);
    ASSERT_EQ(1790, a);
}

TEST(BigIntMethodsTests, ShiftZero) {
    BigInteger a = 179;
    a.shift(0);
    ASSERT_EQ(179, a);
}

TEST(BigIntMethodsTests, ShiftNegative) {
    BigInteger a = -179;
    a.shift(2);
    ASSERT_EQ(-17900, a);
}

TEST(BigIntMethodsTests, ShiftZeroValue) {
    BigInteger a = 0;
    a.shift(30);
    ASSERT_EQ(0, a);
    ASSERT_EQ("0", a.toString());
}

TEST(BigIntMethodsTests, ShiftTime) {
    int total_time = 0;
    int time_treshold = 1000;
    for (size_t i = 0; i < 100; ++i) {
        BigInteger a = random_bigint(100'000);
        Timer T;
        T.start();
        a.shift(100'000);
        T.finish();
        total_time += T.get_time_milliseconds();
        ASSERT_LE(total_time, time_treshold);
    }
}
*/