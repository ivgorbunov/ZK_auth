#pragma once

#include <random>
#include "moduled_bigint.hpp"

std::mt19937_64 rnd(134092830);

// both included
long long random_ll(long long from, long long to) {
  long long diff = to - from;
  return from + rnd() % (diff + 1);
}

// both included
BigInteger random_big_integer(BigInteger from, BigInteger to) {
  BigInteger diff = to - from;
  if (diff.is_zero()) {
    return from;
  }
  // now we need a random number from 0 to diff
  // we generate random digits:
  // for most significant digit group x : from 0 to x
  // for all the other digit groups: from 0 to BASE-1
  // while the number is greater than diff, repeat
  std::vector<long long> digit_groups(diff.digit_groups.size());
  while (true) {
    for (size_t i = 0; i + 1 < diff.digit_groups.size(); ++i) {
      digit_groups[i] = random_ll(0, BigInteger::BASE - 1);
    }
    digit_groups.back() = random_ll(0, diff.digit_groups.back());
    if (BigInteger::compare_digit_groups(digit_groups, diff.digit_groups) !=
        std::strong_ordering::greater) {
      break;
    }
  }
  return from + BigInteger(std::move(digit_groups), true);
}

ModuledBigInt GetRandomNumber() {
  ModuledBigInt md{1};
  ModuledBigInt ans{0};

  // do {
  for (size_t i = 0; i < 100; i++) {
    ans = (ans + (md * BigInteger(rnd())));
    md *= BigInteger(10);
  }
  //} while ((ans.value % P == 0) || (ans.value % Q == 0));
  // std::cout << "-> " << ans.value << std::endl;
  return ans;
}