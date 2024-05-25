#pragma once

#include "bigint.hpp"

BigInteger binary_power(BigInteger a, BigInteger N, const BigInteger& mod);

// probabilistic test
// returns true for a composite number with probability < 1/4
bool is_prime_miller_rabin_test(const BigInteger& N, size_t iter);

// fast test, might return true for a composite number
bool is_prime_effective(const BigInteger& N);

// big prime number, 4k+3
BigInteger generate_big_prime(size_t);
