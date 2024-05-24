#pragma once

#include "primality/primality.hpp"
#include "test_helper.hpp"

// O(sqrt(p))
bool is_prime_dumb(long long p) {
  if (p == 1) {
    return false;
  }
  long long i = 2;
  while (i * i <= p) {
    if (p % i == 0) {
      return false;
    }
    i += 1;
  }
  return true;
}

TEST(PrimalityTests, ExtraSmallN) {
  int N = 1e4;
  for (int i = 1; i <= N; ++i) {
    ASSERT_EQ(is_prime_dumb(i), is_prime_effective(i));
  }
}

TEST(PrimalityTests, SmallN) {
  int N = 3e5;
  std::vector<int> min_prime(N);
  std::vector<int> primes;
  for (int i = 2; i < N; ++i) {
    if (min_prime[i] == 0) {
      min_prime[i] = i;
      primes.emplace_back(i);
    }
    for (size_t j = 0;
         j < primes.size() && primes[j] <= min_prime[i] && i * primes[j] < N;
         ++j) {
      min_prime[primes[j] * i] = primes[j];
    }
  }
  for (int i = 1; i < N; ++i) {
    ASSERT_EQ(min_prime[i] == i, is_prime_effective(i));
  }
}

TEST(PrimalityTests, NotSoSmallN) {
  for (size_t step = 0; step < 20; ++step) {
    long long cur = abs(random_value()) + 1;
    for (size_t cnt = 0; cnt < 100; ++cnt) {
      ASSERT_EQ(is_prime_dumb(cur + cnt), is_prime_effective(cur + cnt));
    }
  }
}