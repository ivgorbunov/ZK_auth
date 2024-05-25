#include "primality.hpp"

#include "rand.hpp"

BigInteger binary_power(BigInteger a, BigInteger N, const BigInteger& mod) {
  BigInteger ans(1);
  std::vector<bool> binary = N.to_binary();
  for (size_t i = 0; i < binary.size(); ++i) {
    if (binary[i]) {
      ans *= a;
      ans %= mod;
    }
    a *= a;
    a %= mod;
  }
  return ans;
}

bool is_prime_miller_rabin_test(const BigInteger& N, size_t iter = 1) {
  if (N % 2 == 0) {
    return N == 2;
  }
  if (N < 9) {
    return N != 1;
  }
  // now N >= 9
  BigInteger d = N;
  --d;
  size_t s = 0;
  while (true) {
    auto [q, r] = BigInteger::divide(d, 2);
    if (r == 0) {
      std::swap(d, q);
      ++s;
    } else {
      break;
    }
  }
  // N - 1 = d * 2^s, d is odd
  for (size_t i = 0; i < iter; ++i) {
    BigInteger A = random_big_integer(2, N - 2);
    BigInteger x = binary_power(A, d, N);
    for (size_t j = 0; j < s; ++j) {
      BigInteger y = (x * x) % N;
      if (y == 1 && x != 1 && x != N - 1) {
        return false;
      }
      std::swap(x, y);
    }
    if (x != 1) {
      return false;
    }
  }
  return true;
}

bool is_prime_effective(const BigInteger& N) {
  // calculated Eratosthenes Sieve up to M
  static const int M = 10000;
  static std::vector<int> min_prime;
  static std::vector<int> primes;

  if (min_prime.empty()) {
    // precalc
    min_prime.resize(M);
    for (int i = 2; i < M; ++i) {
      if (!min_prime[i]) {
        min_prime[i] = i;
        primes.emplace_back(i);
      }
      for (size_t j = 0;
           j < primes.size() && primes[j] <= min_prime[i] && primes[j] * i < M;
           ++j) {
        min_prime[primes[j] * i] = primes[j];
      }
    }
  }

  if (N < M) {
    long long n = static_cast<long long>(N);
    return min_prime[n] == n;
  }

  for (size_t j = 0; j < 30; ++j) {
    if (N % primes[j] == 0) {
      return false;
    }
  }

  // the probability of error ~1/1e6
  return is_prime_miller_rabin_test(N, 20);
}

BigInteger generate_big_prime(size_t len) {
  BigInteger start = random_big_integer(1, BigInteger::power(10, len));
  long long r = start % 4;
  start += 3 - r;
  // now start is 4k+3
  while (!is_prime_effective(start)) {
    start += 4;
  }
  return start;
}
