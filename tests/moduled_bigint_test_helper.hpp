#pragma once

#include "moduled_bigint.hpp"
#include "test_helper.hpp"

#define TEST_MODULED_SAME_OPERATOR(testingOp, correctOp) \
  for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {         \
    BigInteger first = test_random_bigint(100);          \
    BigInteger second = test_random_bigint(100);         \
    BigInteger result = first testingOp second;          \
    first correctOp second;                              \
    ASSERT_EQ(first, result);                            \
  }

#define CHECK_MODULED_AGREED_WITH_INT(op)                                    \
  for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {                             \
    long long a = test_random_value();                                       \
    long long b = test_random_value();                                       \
    long long n(ModuledBigInt::N);                                           \
    ModuledBigInt abi = BigInteger(a);                                       \
    ModuledBigInt bbi = BigInteger(b);                                       \
    ASSERT_EQ((abi op bbi).get_value(), BigInteger(((a op b) % n + n) % n)); \
  }

#define CHECK_MODULED_AGREED_WITH_BIGINT(op)         \
  for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) {     \
    BigInteger a = test_random_bigint(100);          \
    BigInteger b = test_random_bigint(100);          \
    ModuledBigInt abi = BigInteger(a);               \
    ModuledBigInt bbi = BigInteger(b);               \
    BigInteger should = (a op b) % ModuledBigInt::N; \
    if (should.is_negative()) {                      \
      should += ModuledBigInt::N;                    \
    }                                                \
    ASSERT_EQ((abi op bbi).get_value(), should);     \
  }
