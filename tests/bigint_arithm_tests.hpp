#pragma once

#include "bigint.hpp"
#include "bigint_test_helper.hpp"

TEST(BigIntOperatorTests, PlusRandom) { TEST_SAME_OPERATOR(+, +=); }

TEST(BigIntOperatorTests, PlusMemory){CHECK_OPERATOR_ALLOCATIONS(+, 1)}

TEST(BigIntOperatorTests, MinusRandom) {
  TEST_SAME_OPERATOR(-, -=);
}

TEST(BigIntOperatorTests, MinusMemory) { CHECK_OPERATOR_ALLOCATIONS(-, 1); }

TEST(BigIntOperatorTests, MultRandom) { TEST_SAME_OPERATOR(*, *=); }

TEST(BigIntOperatorTests, MultMemory) { CHECK_OPERATOR_ALLOCATIONS(*, 3); }

TEST(BigIntOperatorTests, DivRandom) { TEST_SAME_OPERATOR(/, /=); }

TEST(BigIntOperatorTests, DivMultRandom) {
  std::vector<std::pair<int, int>> length_pairs = {
      {500, 200}, {500, 5}, {100, 50}, {179, 57},
      {57, 179},  {33, 34}, {33, 33},  {228, 179}};
  for (auto [l1, l2] : length_pairs) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
      BigInteger first = test_random_bigint(l1);
      BigInteger second = test_random_bigint(l2);
      auto [quot, rem] = BigInteger::divide(first, second);
      ASSERT_TRUE(BigInteger(0) <= rem);
      ASSERT_TRUE(rem < second);
      ASSERT_EQ(first, quot * second + rem);
    }
  }
}

TEST(BigIntOperatorTests, DivMultLLRandom) {
  std::vector<int> lengths = {1, 2, 3, 7, 11, 179, 57, 555, 719};
  for (auto l : lengths) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
      BigInteger first = test_random_bigint(l);
      long long second = i == 0 ? 1 : abs(test_random_value()) + 1;
      auto quot = first / second;
      auto rem = first % second;
      ASSERT_TRUE(BigInteger(0) <= rem);
      ASSERT_TRUE(rem < second);
      ASSERT_EQ(first, quot * second + rem);
    }
  }
}

TEST(BigIntOperatorTests, DivMultLLRandom2) {
  std::vector<int> lengths = {1, 2, 3, 7, 11, 179, 57, 555, 719};
  for (auto l : lengths) {
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
      BigInteger first = test_random_bigint(l);
      long long second = i == 0 ? 1 : abs(test_random_value()) + 1;
      auto [quot, rem] = BigInteger::divide(first, second);
      ASSERT_TRUE(BigInteger(0) <= rem);
      ASSERT_TRUE(rem < second);
      ASSERT_EQ(first, quot * second + rem);
    }
  }
}

TEST(BigIntOperatorTests, DivMemory) { CHECK_OPERATOR_ALLOCATIONS(/, 2); }

TEST(BigIntOperatorTests, ModRandom) { TEST_SAME_OPERATOR(%, %=); }

TEST(BigIntOperatorTests, ModMemory) { CHECK_OPERATOR_ALLOCATIONS(%, 4); }

TEST(BigIntOperatorTests, PlusEQ) {
  BigInteger a = 179;
  a += 179;

  ASSERT_EQ(179 + 179, a);
}

TEST(BigIntOperatorTests, PlusEQNeg) {
  BigInteger a = -179;
  a += 57;

  ASSERT_EQ(57 - 179, a);
}

TEST(BigIntOperatorTests, PlusEQNegNeg) {
  BigInteger a = -179;
  a += -57;

  ASSERT_EQ(-179 - 57, a);
}

TEST(BigIntOperatorTests, PlusEQNegPos) {
  BigInteger a = -57;
  a += 179;

  ASSERT_EQ(179 - 57, a);
}

TEST(BigIntOperatorTests, PlusEQPosNeg) {
  BigInteger a = 179;
  a += -57;
  ASSERT_EQ(179 - 57, a);
}

TEST(BigIntOperatorTests, PlusEQPosNegOverflow) {
  BigInteger a = 57;
  a += -179;

  ASSERT_EQ(57 - 179, a);
}

TEST(BigIntOperatorTests, PlusEQRandom) {
  for (auto i = 0; i < RANDOM_TRIES_COUNT; ++i) {
    long long first = test_random_value();
    long long second = test_random_value();
    BigInteger a = first;
    a += second;
    ASSERT_EQ(first + second, a);
  }
}

TEST(BigIntOperatorTests, PlusEQTime) {
  int total_time = 0;
  int time_treshold = 1000000;
  for (int i = 0; i < 2000; ++i) {
    BigInteger first = test_random_bigint(1e4);
    BigInteger second = test_random_bigint(1e4);
    Timer T;
    T.start();
    first += second;
    T.finish();
    total_time += T.get_time_microseconds();
    ASSERT_LE(total_time, time_treshold) << i << " iteration of 1000";
  }
}

TEST(BigIntOperatorTests, PlusEqMemory) { CHECK_OPERATOR_ALLOCATIONS(+=, 1); }

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
    long long first = test_random_value();
    long long second = test_random_value();
    BigInteger a = first;
    a -= second;
    ASSERT_EQ(first - second, a);
  }
}

TEST(BigIntOperatorTests, MinusEqMemory) { CHECK_OPERATOR_ALLOCATIONS(-=, 1); }

void test_mult(long long a, long long b) {
  BigInteger bi = a;
  bi *= b;

  ASSERT_EQ(a * b, bi);
}

TEST(BigIntOperatorTests, TimesEQ) { test_mult(57, 4); }

TEST(BigIntOperatorTests, TimesEQNeg) { test_mult(-57, 4); }

TEST(BigIntOperatorTests, TimesEQNegNeg) { test_mult(-57, -4); }

TEST(BigIntOperatorTests, TimesEQPosNeg) { test_mult(57, -4); }

TEST(BigIntOperatorTests, TimesEQZero) { test_mult(1791791791, 0); }

TEST(BigIntOperatorTests, TimesEQZeroNeg) { test_mult(-1791791791, 0); }

TEST(BigIntOperatorTests, TimesEQRandom) {
  for (auto i = 0; i < RANDOM_TRIES_COUNT; ++i) {
    test_mult(test_random_value(), test_random_value());
  }
}

TEST(BigIntOperatorTests, TimesEQTime) {
  int total_time = 0;
  int time_treshold = 3000;

  for (int i = 0; i < 100; ++i) {
    auto first = test_random_bigint(1e4);
    auto second = test_random_bigint(1e4);
    Timer T;
    T.start();
    first *= second;
    T.finish();
    total_time += T.get_time_milliseconds();
    ASSERT_LE(total_time, time_treshold) << i << " of 100 iterations";
  }
}

TEST(BigIntOperatorTests, TimesEqMemory) { CHECK_OPERATOR_ALLOCATIONS(*=, 2); }

void test_division(long long left, long long right) {
  BigInteger a = left;
  a /= right;

  ASSERT_EQ(left / right, a);
}

TEST(BigIntOperatorTests, DivEQ) { test_division(179, 57); }

TEST(BigIntOperatorTests, DivNeg) { test_division(-179, 57); }

TEST(BigIntOperatorTests, DivPosNeg) { test_division(-179, 57); }

TEST(BigIntOperatorTests, DivNegNeg) { test_division(-179, -57); }

TEST(BigIntOperatorTests, DivEqRandom) {
  for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
    auto left = test_random_value();
    auto right = 0;
    while (right == 0) right = test_random_value();
    test_division(left, right);
  }
}

TEST(BigIntOperatorTests, DivTime) {
  int total_time = 0;
  int time_treshold = 2000;

  for (int i = 0; i < 100; ++i) {
    auto first = test_random_bigint(5e2);
    auto second = test_random_bigint(5e2);
    Timer T;
    T.start();
    first /= second;
    T.finish();
    total_time += T.get_time_milliseconds();
    ASSERT_LE(total_time, time_treshold);
  }
}

TEST(BigIntOperatorTests, DivEqMemory) { CHECK_OPERATOR_ALLOCATIONS(/=, 1); }

void test_modulus(long long left, long long right) {
  BigInteger a = left;
  a %= right;

  ASSERT_EQ(left % right, a);
}

TEST(BigIntOperatorTests, Mod) { test_modulus(179, 57); }

TEST(BigIntOperatorTests, ModNeg) { test_modulus(-179, 57); }

TEST(BigIntOperatorTests, ModPosNeg) { test_modulus(179, -57); }

TEST(BigIntOperatorTests, ModNegNeg) { test_modulus(-179, -179); }

TEST(BigIntOperatorTests, ModEqRandom) {
  for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
    long long left = test_random_value();
    long long right = 0;
    while (right == 0) right = test_random_value();

    test_modulus(left, right);
  }
}

TEST(BigIntOperatorTests, ModEqTime) {
  int total_time = 0;
  int time_treshold = 2000;

  for (int i = 0; i < 100; ++i) {
    auto first = test_random_bigint(5e2);
    auto second = test_random_bigint(5e2);
    Timer T;
    T.start();
    first %= second;
    T.finish();
    total_time += T.get_time_milliseconds();
    ASSERT_LE(total_time, time_treshold);
  }
}

TEST(BigIntOperatorTests, ModEqMemory) { CHECK_OPERATOR_ALLOCATIONS(%=, 3); }

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
  ASSERT_EQ(179, ++ ++a);
  ASSERT_EQ(179, a);
}

TEST(BigIntOperatorTests, LPPlusTime) {
  int total_time = 0;
  int time_treshold = 1500000;

  for (int i = 0; i < 10000; ++i) {
    Timer T;
    BigInteger first = test_random_bigint(9);
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
  ASSERT_EQ(179, -- --a);
  ASSERT_EQ(179, a);
}

TEST(BigIntOperatorTests, LMMinusTime) {
  int total_time = 0;
  int time_treshold = 1500;

  for (int i = 0; i < 100; ++i) {
    Timer T;
    BigInteger first = test_random_bigint(1e2);
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
    BigInteger a = test_random_bigint(500);
    BigInteger b = -a;
    BigInteger c = -b;
    BigInteger d = -c;
    ASSERT_EQ(a, c);
    ASSERT_EQ(b, d);
    ASSERT_EQ(a + b, 0);
  }
}

TEST(BigIntMethodTests, Power) {
  BigInteger a = BigInteger::power(2, 6);
  ASSERT_EQ(a, 64);
}

TEST(BigIntMethodTests, PowerZero) {
  BigInteger a = BigInteger::power(2, 0);
  ASSERT_EQ(1, a);
}

TEST(BigIntMethodTests, PowerBig) {
  BigInteger a = BigInteger::power(2, 16);
  ASSERT_EQ(65536, a);
}

TEST(BigIntMethodTests, PowerOne) {
  BigInteger a = BigInteger::power(1, 1791791791);
  ASSERT_EQ(1, a);
}

TEST(BigIntMethodTests, NegPower) {
  BigInteger a = BigInteger::power(-2, 5);
  ASSERT_EQ(-32, a);
}

TEST(BigIntMethodTests, NegEvenPower) {
  BigInteger a = BigInteger::power(-2, 8);
  ASSERT_EQ(256, a);
}

TEST(BigIntMethodTests, ZeroPower) {
  BigInteger a = BigInteger::power(0, 1791791791);
  ASSERT_EQ(0, a);
}

TEST(BigIntMethodTests, PowerSameAsMult) {
  for (size_t i = 0; i < 20; ++i) {
    BigInteger a = test_random_bigint(20);
    size_t n = abs(test_random_value()) % 10;
    BigInteger should = 1;
    for (size_t j = 0; j < n; ++j) {
      should *= a;
    }
    ASSERT_EQ(should, BigInteger::power(a, n));
  }
}

TEST(BigIntMethodTests, PowerTime) {
  int time_treshold = 2500;
  int total_time = 0;

  for (size_t i = 0; i < 100; ++i) {
    Timer T;
    T.start();
    BigInteger::power(test_random_bigint(3), 179);
    T.finish();
    total_time += T.get_time_milliseconds();

    ASSERT_LE(total_time, time_treshold) << i << " iteration of 100";
  }
}

TEST(BigIntMethodTests, IsZeroPositive) {
  BigInteger a = 179;
  ASSERT_FALSE(a.is_zero());
}

TEST(BigIntMethodTests, IsZeroNegative) {
  BigInteger a = -179;
  ASSERT_FALSE(a.is_zero());
}

TEST(BigIntMethodTests, IsZeroZero) {
  BigInteger a = 0;
  ASSERT_TRUE(a.is_zero());
}

TEST(BigIntMethodTests, IsZeroTen) {
  BigInteger a = 10;
  ASSERT_FALSE(a.is_zero());
}

TEST(BigIntMethodTests, IsNegativePositive) {
  BigInteger a = 179;
  ASSERT_FALSE(a.is_negative());
}

TEST(BigIntMethodTests, IsNegativeZero) {
  BigInteger a = 0;
  ASSERT_FALSE(a.is_negative());
}

TEST(BigIntMethodTests, IsNegativeNegative) {
  BigInteger a = -1;
  ASSERT_TRUE(a.is_negative());
}

TEST(BigIntMethodTests, IsNegativeAfterSum) {
  BigInteger a = -11;
  a += 11;
  ASSERT_FALSE(a.is_negative());
}

TEST(BigIntOperatorTests, OperatorsToItself) {
  for (int i = 0; i < 20; ++i) {
    BigInteger x = test_random_bigint(100 + i);
    BigInteger a = x;
    a += a;
    ASSERT_EQ(a, x + x);
    a = x;
    a -= a;
    ASSERT_EQ(a, 0);
    a = x;
    a *= a;
    ASSERT_EQ(a, x * x);
    a = x;
    a /= a;
    ASSERT_EQ(a, 1);
    a = x;
    a %= a;
    ASSERT_EQ(a, 0);
  }
}

TEST(BigIntMethodTests, ToBinaryTest) {
  for (int small = 0; small <= 1024; ++small) {
    int copy = small;
    vector<bool> should;
    while (copy) {
      should.emplace_back(copy & 1);
      copy >>= 1;
    }
    ASSERT_EQ(should, BigInteger(small).to_binary());
  }
  for (int i = 0; i < 100; ++i) {
    long long cur = test_random_value();
    long long copy = abs(cur);
    vector<bool> should;
    while (copy) {
      should.emplace_back(copy & 1ll);
      copy >>= 1;
    }
    ASSERT_EQ(should, BigInteger(cur).to_binary());
  }
  for (int i = 0; i < 20; ++i) {
    int len = 179 + i * 57;
    vector<bool> should(len);
    for (int j = 0; j < len; ++j) should[j] = test_random_value() % 2;
    should.emplace_back(1);
    BigInteger value = 0;
    for (int j = 0; j <= len; ++j) {
      value *= 2;
      value += int(should[len - j]);
    }
    auto ans = value.to_binary();
    ASSERT_EQ(value.to_binary(), should);
  }
}
