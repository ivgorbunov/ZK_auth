#pragma once

#include "test_helper.hpp"

#include "moduled_bigint.hpp"

#define TEST_MODULED_SAME_OPERATOR(testingOp, correctOp) \
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) { \
        BigInteger first = random_bigint(100); \
        BigInteger second = random_bigint(100); \
        BigInteger result = first testingOp second; \
        first correctOp second; \
        ASSERT_EQ(first, result); \
    }

#define CHECK_MODULED_AGREED_WITH_INT(op) \
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) { \
        long long a = random_value(); \
        long long b = random_value(); \
        long long n(ModuledBigInt::N); \
        ModuledBigInt abi = BigInteger(a); \
        ModuledBigInt bbi = BigInteger(b); \
        ASSERT_EQ((abi op bbi).get_value(), BigInteger(((a op b) % n + n) % n)); \
    }

#define CHECK_MODULED_AGREED_WITH_BIGINT(op) \
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) { \
        BigInteger a = random_bigint(100); \
        BigInteger b = random_bigint(100); \
        ModuledBigInt abi = BigInteger(a); \
        ModuledBigInt bbi = BigInteger(b); \
        BigInteger should = (a op b) % ModuledBigInt::N; \
        if (should.is_negative()) { \
            should += ModuledBigInt::N; \
        } \
        ASSERT_EQ((abi op bbi).get_value(), should); \
    }


