#pragma once

#include "bigint_test_helper.hpp"

TEST(BigIntOperatorTests, EqTrue) {
  BigInteger a = 179;
  BigInteger b = 179;

  ASSERT_TRUE(a == b);
}

TEST(BigIntOperatorTests, EqFalse) {
  BigInteger a = 179;
  BigInteger b = 57;

  ASSERT_FALSE(a == b);
}

TEST(BigIntOperatorTests, EqSignedNeq) {
  BigInteger a = 179;
  BigInteger b = -179;

  ASSERT_FALSE(a == b);
}

TEST(BigIntOperatorTests, EqTime) {
  std::vector<BigInteger> testCases;
  for (int i = 0; i < 179; ++i) {
    testCases.push_back(random_bigint(1000));
  }

  int total_time = 0;
  int time_treshold = 1000000;
  for (int i = 0; i < 179; ++i) {
    Timer T;
    T.start();
    for (int j = i + 1; j < 179; ++j) {
      testCases[i] == testCases[j];
    }
    T.finish();
    total_time += T.get_time_microseconds();
    ASSERT_LE(total_time, time_treshold);
  }
}

TEST(BigIntOperatorTests, EqMemory) { CHECK_OPERATOR_ALLOCATIONS(==, 0); }

TEST(BigIntOperatorTests, NeqRandom) {
  for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {
    BigInteger first = random_bigint(1000);
    BigInteger second = random_bigint(1000);
    ASSERT_EQ(first == second, !(first != second));
  }
}

TEST(BigIntOperatorTests, NeqMemory) { CHECK_OPERATOR_ALLOCATIONS(!=, 0); }

TEST(BigIntOperatorTests, SpaceshipG) {
  BigInteger a = 179;
  BigInteger b = 57;

  auto cmp = a <=> b;
  ASSERT_EQ(std::strong_ordering::greater, cmp);
}

TEST(BigIntOperatorTests, SpaceshipL) {
  BigInteger a = 57;
  BigInteger b = 179;

  ASSERT_EQ(std::strong_ordering::less, a <=> b);
}

TEST(BigIntOperatorTests, SpacesipEq) {
  BigInteger a = 179;
  BigInteger b = 179;

  ASSERT_EQ(std::strong_ordering::equivalent, a <=> b);
}

TEST(BigIntOperatorTests, SpaceshipGNeg) {
  BigInteger a = 57;
  BigInteger b = -179;

  ASSERT_EQ(std::strong_ordering::greater, a <=> b);
}

TEST(BigIntOperatorTests, SpaceshipLNeg) {
  BigInteger a = -179;
  BigInteger b = 57;

  ASSERT_EQ(std::strong_ordering::less, a <=> b);
}

TEST(BigIntOperatorTests, SpaceshipTwoNeg) {
  BigInteger a = -179;
  BigInteger b = -57;

  ASSERT_EQ(std::strong_ordering::less, a <=> b);
}

TEST(BigIntOperatorTests, SpaceshipTwoNegEq) {
  BigInteger a = -179;
  BigInteger b = -179;

  ASSERT_EQ(std::strong_ordering::equivalent, a <=> b);
}

TEST(BigIntOperatorTests, SpaceshipSign) {
  BigInteger a = 179;
  BigInteger b = -179;

  ASSERT_EQ(std::strong_ordering::greater, a <=> b);
}

TEST(BigIntOperatorTests, SpaceshipTime) {
  int total_time = 0;
  int time_treshold = 1000000;
  for (int i = 0; i < 1000; ++i) {
    BigInteger a = random_bigint(1e4);
    BigInteger b = random_bigint(1e4 - 30);

    Timer T;
    T.start();
    a <=> b;
    T.finish();
    total_time += T.get_time_microseconds();
    ASSERT_LE(total_time, time_treshold);
  }
}

TEST(BigIntOperatorTests, SpaceshipMemory) {
  CHECK_OPERATOR_ALLOCATIONS(<=>, 0);
}

TEST(BigIntOperatorTests, Le) { CHECK_AGREED_WITH_INT(<); }

TEST(BigIntOperatorTests, Leq) { CHECK_AGREED_WITH_INT(<=); }

TEST(BigIntOperatorTests, Ge) { CHECK_AGREED_WITH_INT(>); }

TEST(BigIntOperatorTests, Geq) { CHECK_AGREED_WITH_INT(>=); }