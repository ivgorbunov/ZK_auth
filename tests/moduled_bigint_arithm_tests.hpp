#pragma once

#include <gtest/gtest.h>

#include "moduled_bigint.hpp"
#include "moduled_bigint_test_helper.hpp"

template <typename Test>
void check_test_multiple_small_n(Test test) {
  std::vector<std::string> small_mods = {
      "2",   "3",    "35",     "56",       "179",     "323",
      "665", "1000", "316071", "72718615", "8388608", "404274424"};
  for (auto mod : small_mods) {
    ModuledBigInt::N = BigInteger(mod);
    test();
  }
}

TEST(ModuledBigIntSmallNTests, SimpleTests) {
  check_test_multiple_small_n([]() {
    long long n(ModuledBigInt::N);
    for (int i = 0; i < 100; ++i) {
      long long x = random_value();
      long long should = x % n;
      if (should < 0) {
        should += n;
      }
      ASSERT_EQ(ModuledBigInt(x).get_value(), should);
    }
    for (int i = 0; i < 100; ++i) {
      BigInteger big_value = random_bigint(100);
      ModuledBigInt cur = big_value;
      ASSERT_TRUE(0 <= cur.get_value());
      ASSERT_TRUE(cur.get_value() < ModuledBigInt::N);
    }
  });
}

TEST(ModuledBigIntSmallNTests, SameOps) {
  check_test_multiple_small_n([]() {
    TEST_MODULED_SAME_OPERATOR(+, +=);
    TEST_MODULED_SAME_OPERATOR(-, -=);
    TEST_MODULED_SAME_OPERATOR(*, *=);
  });
}

TEST(ModuledBigIntSmallNTests, AgreedWithIntOps) {
  check_test_multiple_small_n([]() {
    CHECK_MODULED_AGREED_WITH_INT(+);
    CHECK_MODULED_AGREED_WITH_INT(-);
    CHECK_MODULED_AGREED_WITH_INT(*);
  });
}

TEST(ModuledBigIntSmallNTests, UnaryMinus) {
  check_test_multiple_small_n([]() {
    for (size_t i = 0; i < 20; ++i) {
      ModuledBigInt a = random_bigint(200);
      ModuledBigInt b = -a;
      ModuledBigInt c = -b;
      ModuledBigInt d = -c;
      ASSERT_EQ(a, c);
      ASSERT_EQ(b, d);
      ASSERT_EQ(a + b, ModuledBigInt());
      ASSERT_EQ(c + d, ModuledBigInt());
    }
  });
}

TEST(ModuledBigIntSmallNTests, EqTests) {
  check_test_multiple_small_n([]() {
    long long n(ModuledBigInt::N);
    std::vector<long long> a(100);
    for (int i = 0; i < 100; ++i) {
      a[i] = random_value() % n;
      if (a[i] < 0) a[i] += n;
    }
    sort(a.begin(), a.end());
    for (size_t i = 0; i + 1 < a.size(); ++i) {
      ModuledBigInt first = a[i];
      ModuledBigInt second = a[i + 1];
      if (a[i] != a[i + 1]) {
        ASSERT_TRUE(first < second);
        ASSERT_TRUE(first != second);
        ASSERT_TRUE(second > first);
        ASSERT_FALSE(first == second);
      } else {
        ASSERT_FALSE(first < second);
        ASSERT_FALSE(first != second);
        ASSERT_FALSE(second > first);
        ASSERT_TRUE(first == second);
      }
    }
    for (size_t i = 0; i < a.size(); ++i) {
      ModuledBigInt cur = a[i];
      ASSERT_EQ(cur, cur);
      ASSERT_EQ(cur, ModuledBigInt(a[i] + n));
      ASSERT_EQ(cur, ModuledBigInt(a[i] + n * n));
      ASSERT_EQ(ModuledBigInt(a[i] + n * 57), ModuledBigInt(a[i] - n * 179179));
      // 1 is not a modulo
      ASSERT_TRUE(cur != ModuledBigInt(a[i] + 1));
      ASSERT_FALSE(cur != cur);
    }
  });
}

long long gcd(long long a, long long b) {
  while (b) {
    a %= b;
    std::swap(a, b);
  }
  return a;
}

long long pow_mod(long long a, long long n, long long mod) {
  long long ans = 1;
  while (n) {
    if (n & 1ll) {
      ans = ans * a % mod;
    }
    n /= 2;
    a = a * a % mod;
  }
  return ans;
}

TEST(ModuledBigIntSmallNTests, InverseTests) {
  check_test_multiple_small_n([]() {
    long long n(ModuledBigInt::N);
    if (n <= 100) {
      for (int r = 0; r < n; ++r) {
        int pos_inv = -1;
        for (int i = 0; i < n; ++i) {
          if ((i * r) % n == 1) {
            pos_inv = i;
          }
        }
        if (pos_inv != -1) {
          ASSERT_EQ(ModuledBigInt(r).inversed(), ModuledBigInt(pos_inv));
        } else {
          ASSERT_EQ(ModuledBigInt(r).inversed(), ModuledBigInt());
        }
      }
    } else if (n <= 1000) {
      int phi = 0;
      for (int r = 0; r < n; ++r) {
        int g = gcd(r, n);
        if (g == 1) {
          ++phi;
        }
      }
      for (int r = 0; r < n; ++r) {
        int g = gcd(r, n);
        if (g == 1) {
          int inv = pow_mod(r, phi - 1, n);
          ASSERT_EQ(ModuledBigInt(r).inversed(), inv);
        } else {
          ASSERT_EQ(ModuledBigInt(r).inversed(), 0);
        }
      }
    } else {
      long long start = abs(random_value());
      for (int i = 0; i < 1000; ++i) {
        long long cur = start + i;
        long long g = gcd(cur, n);
        if (g == 1) {
          BigInteger inv = ModuledBigInt(cur).inversed().get_value();
          ASSERT_TRUE(0 <= inv);
          ASSERT_TRUE(inv < ModuledBigInt::N);
          long long inv_ll = static_cast<long long>(inv);
          ASSERT_TRUE((cur * inv_ll) % n == 1);
        } else {
          ASSERT_EQ(ModuledBigInt(cur).inversed(), 0);
        }
      }
    }
  });
}

template <typename Test>
void check_test_multiple_big_n(Test test) {
  std::vector<std::string> big_mods = {
      "151235123512361464326234",
      "180935129857123918879899999999",
      "1000000000000000000000000000000000000000000000",
      "7777777777777777777777777777777777777777777777777777777777777777777",
      "193258612359861235971239857123958123519717123958132235986123598125098170"
      "91863257132579357817258123598123",
      "76660291456752554728676319376664360839564502506391",
      "83597612331266037584520187757"};
  for (auto mod : big_mods) {
    ModuledBigInt::N = BigInteger(mod);
    test();
  }
}

TEST(ModuledBigIntBigNTests, SimpleTests) {
  check_test_multiple_big_n([]() {
    for (int i = 0; i < 100; ++i) {
      BigInteger big_value = random_bigint(100);
      ModuledBigInt cur = big_value;
      ASSERT_TRUE(0 <= cur.get_value());
      ASSERT_TRUE(cur.get_value() < ModuledBigInt::N);
    }
  });
}

TEST(ModuledBigIntBigNTests, SameOps) {
  check_test_multiple_big_n([]() {
    TEST_MODULED_SAME_OPERATOR(+, +=);
    TEST_MODULED_SAME_OPERATOR(-, -=);
    TEST_MODULED_SAME_OPERATOR(*, *=);
  });
}

TEST(ModuledBigIntBigNTests, AgreedWithBigIntOps) {
  check_test_multiple_big_n([]() {
    CHECK_MODULED_AGREED_WITH_BIGINT(+);
    CHECK_MODULED_AGREED_WITH_BIGINT(-);
    CHECK_MODULED_AGREED_WITH_BIGINT(*);
  });
}

TEST(ModuledBigIntBigNTests, UnaryMinus) {
  check_test_multiple_big_n([]() {
    for (size_t i = 0; i < 20; ++i) {
      ModuledBigInt a = random_bigint(200);
      ModuledBigInt b = -a;
      ModuledBigInt c = -b;
      ModuledBigInt d = -c;
      ASSERT_EQ(a, c);
      ASSERT_EQ(b, d);
      ASSERT_EQ(a + b, ModuledBigInt());
      ASSERT_EQ(c + d, ModuledBigInt());
    }
  });
}

TEST(ModuledBigIntBigNTests, EqTests) {
  check_test_multiple_big_n([]() {
    int cnt = 100;
    std::vector<BigInteger> a(cnt);
    for (int i = 0; i < cnt; ++i) {
      a[i] = random_bigint(100) % ModuledBigInt::N;
    }
    sort(a.begin(), a.end());
    for (size_t i = 0; i + 1 < a.size(); ++i) {
      ModuledBigInt first = a[i];
      ModuledBigInt second = a[i + 1];
      if (a[i] != a[i + 1]) {
        ASSERT_TRUE(first < second);
        ASSERT_TRUE(first != second);
        ASSERT_TRUE(second > first);
        ASSERT_FALSE(first == second);
      } else {
        ASSERT_FALSE(first < second);
        ASSERT_FALSE(first != second);
        ASSERT_FALSE(second > first);
        ASSERT_TRUE(first == second);
      }
    }
    for (size_t i = 0; i < a.size(); ++i) {
      ModuledBigInt cur = a[i];
      ASSERT_EQ(cur, cur);
      ASSERT_EQ(cur, ModuledBigInt(a[i] + ModuledBigInt::N));
      ASSERT_EQ(cur, ModuledBigInt(a[i] + ModuledBigInt::N * ModuledBigInt::N));
      ASSERT_EQ(ModuledBigInt(a[i] + ModuledBigInt::N * 57),
                ModuledBigInt(a[i] - ModuledBigInt::N * 179179));
      // 1 is not a modulo
      ASSERT_TRUE(cur != ModuledBigInt(a[i] + 1));
      ASSERT_FALSE(cur != cur);
    }
  });
}

BigInteger gcd(BigInteger a, BigInteger b) {
  while (b) {
    a %= b;
    swap(a, b);
  }
  return a;
}

TEST(ModuledBigIntBigNTests, InverseTests) {
  check_test_multiple_big_n([]() {
    BigInteger start = random_bigint(100);
    for (int i = 0; i < 100; ++i) {
      BigInteger cur = start + i;
      BigInteger g = gcd(cur, ModuledBigInt::N);
      if (g == 1) {
        BigInteger inv = ModuledBigInt(cur).inversed().get_value();
        ASSERT_TRUE(0 <= inv);
        ASSERT_TRUE(inv < ModuledBigInt::N);
        ASSERT_TRUE((cur * inv) % ModuledBigInt::N == 1);
      } else {
        ASSERT_EQ(ModuledBigInt(cur).inversed().get_value(), 0);
      }
    }
  });
}
