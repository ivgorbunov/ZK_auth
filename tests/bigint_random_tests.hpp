#include "bigint_test_helper.hpp"
#include "rand.hpp"

TEST(BigIntRandomTests, BasicCorrectRandom) {
  std::vector<std::pair<int, int>> lengths = {{100, 50},  {2, 1},   {1, 1},
                                              {30, 10},   {25, 11}, {179, 1},
                                              {100, 200}, {30, 30}};
  for (auto [l1, l2] : lengths) {
    BigInteger from = test_random_bigint(l1);
    BigInteger to = from + test_random_bigint(l2);
    for (int i = 0; i < 100; ++i) {
      BigInteger rnd = random_big_integer(from, to);
      ASSERT_TRUE(from <= rnd);
      ASSERT_TRUE(rnd <= to);
    }
  }
}

TEST(BigIntRandomTests, CorrectDistributionSmallD) {
  std::vector<std::pair<int, int>> len_diff = {
      {100, 3}, {1, 7}, {2, 1}, {179, 0}, {15, 4}, {123, 10}, {2, 2}};
  for (auto [len, diff] : len_diff) {
    BigInteger from = test_random_bigint(len);
    BigInteger to = from + diff;
    vector<int> cnt(diff + 1);
    const int samples = 100;
    for (int i = 0; i < samples; ++i) {
      BigInteger rnd = random_big_integer(from, to);
      ASSERT_TRUE(from <= rnd);
      ASSERT_TRUE(rnd <= to);
      BigInteger cur_d = rnd - from;
      ++cnt[static_cast<long long>(cur_d)];
    }
    int avg_cnt = samples / (diff + 1);
    int max_diff = 0;
    for (int real_cnt : cnt) {
        max_diff = max(max_diff, abs(avg_cnt - real_cnt));
    }
    ASSERT_TRUE(max_diff <= 15);
    // distribution is pretty uniform
  }
}