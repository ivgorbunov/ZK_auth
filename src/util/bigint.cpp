#include "bigint.hpp"

#include "fft.hpp"

/*
 * digit_groups consist of blocks of BASELEN digits
 * positive represents the sign of the number
 * digit_groups are numbered from least significant to most significant
 * digit_groups can't end with zeroes
 * 0 is always like this: digit_groups is empty, positive is true
 */

bool is_digit(char c) { return '0' <= c && c <= '9'; }

void BigInteger::fix_zero_digits() {
  while (!digit_groups.empty() && digit_groups.back() == 0) {
    digit_groups.pop_back();
  }
  if (digit_groups.empty()) {
    positive = true;
  }
}

BigInteger::BigInteger() : positive(true) {}

BigInteger::BigInteger(const std::string& number) : positive(true) {
  if (number.empty()) {
    throw std::logic_error("Empty string in BigInteger constructor");
  }
  size_t start = 0;
  if (number[0] == '-') {
    positive = false;
    start = 1;
  } else if (number[0] == '+') {
    start = 1;
  } else if (!is_digit(number[0])) {
    throw std::logic_error(
        "Got strange symbol in BigInteger string constructor");
  }
  size_t i = 0;
  while (number.size() > i + start) {
    size_t to = std::min(i + BASELEN, number.size() - start);
    long long cur_number = 0;
    long long power = 1;
    for (size_t j = i; j < to; ++j) {
      char digit = number[number.size() - j - 1];
      if (!is_digit(digit)) {
        throw std::logic_error(
            "Got strange symbol in BigIntger string constructor");
      }
      cur_number += (digit - '0') * power;
      power *= 10;
    }
    digit_groups.emplace_back(cur_number);
    i = to;
  }
  fix_zero_digits();
}

BigInteger::BigInteger(long long number) : positive(number >= 0) {
  if (positive) {
    while (number) {
      digit_groups.emplace_back(number % BASE);
      number /= BASE;
    }
  } else {
    while (number) {
      digit_groups.emplace_back(-(number % BASE));
      number /= BASE;
    }
  }
}

BigInteger::operator long long() const {
  long long ans = 0;
  for (size_t i = 0; i < digit_groups.size(); ++i) {
    ans = ans * BASE + digit_groups[digit_groups.size() - i - 1];
  }
  if (!positive) {
    ans *= -1;
  }
  return ans;
}

BigInteger::operator std::string() const {
  std::string ans;
  if (!positive) {
    ans += '-';
  }
  for (size_t i = 0; i < digit_groups.size(); ++i) {
    std::string block = to_string(digit_groups[digit_groups.size() - i - 1]);
    if (i != 0) {
      for (size_t j = block.size(); j < BASELEN; ++j) {
        ans += '0';
      }
    }
    ans += block;
  }
  // zero
  if (digit_groups.empty()) {
    ans += '0';
  }
  return ans;
}

bool BigInteger::is_zero() const { return digit_groups.empty(); }

bool BigInteger::is_positive() const { return positive && !is_zero(); }

bool BigInteger::is_negative() const { return !positive; }

BigInteger::operator bool() const { return !is_zero(); }

BigInteger::BigInteger(BigInteger&& other)
    : digit_groups(std::move(other.digit_groups)), positive(other.positive) {
  // clear up other
  other.positive = true;
  other.digit_groups.clear();
}

BigInteger& BigInteger::operator=(BigInteger&& other) {
  positive = other.positive;
  digit_groups = std::move(other.digit_groups);
  // clear up other
  other.positive = true;
  other.digit_groups.clear();
  return *this;
}

std::strong_ordering BigInteger::compare_digit_groups(
    const std::vector<long long>& digit_groups_left,
    const std::vector<long long>& digit_groups_right) {
  if (digit_groups_left.size() != digit_groups_right.size()) {
    return digit_groups_left.size() <=> digit_groups_right.size();
  }
  for (size_t i = 0; i < digit_groups_left.size(); ++i) {
    long long left_block = digit_groups_left[digit_groups_left.size() - i - 1];
    long long right_block =
        digit_groups_right[digit_groups_right.size() - i - 1];
    if (left_block != right_block) {
      return left_block <=> right_block;
    }
  }
  return std::strong_ordering::equal;
}

std::strong_ordering BigInteger::operator<=>(const BigInteger& other) const {
  if (positive != other.positive) {
    return positive ? std::strong_ordering::greater
                    : std::strong_ordering::less;
  }
  auto result = compare_digit_groups(digit_groups, other.digit_groups);
  if (positive) {
    return result;
  }
  // reversed otherwise
  return 0 <=> result;
}

void BigInteger::add_with_sign(
    bool same_sign, const std::vector<long long>& other_digit_groups) {
  digit_groups.resize(std::max(digit_groups.size(), other_digit_groups.size()));
  if (same_sign) {
    for (size_t i = 0; i < other_digit_groups.size(); ++i) {
      digit_groups[i] += other_digit_groups[i];
    }
  } else if (compare_digit_groups(digit_groups, other_digit_groups) ==
             std::strong_ordering::less) {
    // extra zeroes might have been added but it does not break anything
    for (size_t i = 0; i < other_digit_groups.size(); ++i) {
      digit_groups[i] = other_digit_groups[i] - digit_groups[i];
    }
    positive ^= 1;
  } else {
    for (size_t i = 0; i < other_digit_groups.size(); ++i) {
      digit_groups[i] -= other_digit_groups[i];
    }
  }
  size_t border = other_digit_groups.size();
  for (size_t i = 0; i < border; ++i) {
    if (digit_groups[i] >= BASE) {
      digit_groups[i] -= BASE;
      border = max(border, i + 2);
      if (i + 1 == digit_groups.size()) {
        digit_groups.emplace_back(1);
      } else {
        ++digit_groups[i + 1];
      }
    }
    if (digit_groups[i] < 0) {
      digit_groups[i] += BASE;
      // it should exits for sure
      --digit_groups[i + 1];
    }
  }
  fix_zero_digits();
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
  add_with_sign(positive ^ other.positive ^ 1, other.digit_groups);
  return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
  add_with_sign(positive ^ other.positive, other.digit_groups);
  return *this;
}

BigInteger operator+(const BigInteger& a, const BigInteger& b) {
  BigInteger copy(a);
  copy += b;
  return copy;
}

BigInteger operator-(const BigInteger& a, const BigInteger& b) {
  BigInteger copy(a);
  copy -= b;
  return copy;
}

BigInteger::BigInteger(const std::vector<long long>& digit_groups,
                       bool positive)
    : digit_groups(digit_groups), positive(positive) {
  fix_zero_digits();
}

BigInteger::BigInteger(std::vector<long long>&& digit_groups, bool positive)
    : digit_groups(std::move(digit_groups)), positive(positive) {
  fix_zero_digits();
}

BigInteger operator*(const BigInteger& a, const BigInteger& b) {
  std::vector<long long> res_digit_groups = FFT::multiply_poly<long long>(
      a.digit_groups.begin(), a.digit_groups.end(), b.digit_groups.begin(),
      b.digit_groups.end());
  for (size_t i = 0; i < res_digit_groups.size(); ++i) {
    if (res_digit_groups[i] >= BigInteger::BASE) {
      // it should exist
      if (i + 1 == res_digit_groups.size()) {
        res_digit_groups.emplace_back();
      }
      res_digit_groups[i + 1] += res_digit_groups[i] / BigInteger::BASE;
      res_digit_groups[i] %= BigInteger::BASE;
    }
  }
  BigInteger res(res_digit_groups, a.positive ^ b.positive ^ 1);
  return res;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
  return *this = ((*this) * other);
}

void BigInteger::add_one_with_sign(bool same_sign) {
  if (same_sign) {
    if (digit_groups.empty()) {
      digit_groups.emplace_back();
    }
    ++digit_groups[0];
    size_t i = 0;
    while (digit_groups[i] >= BASE) {
      if (i + 1 == digit_groups.size()) {
        digit_groups.emplace_back();
      }
      ++digit_groups[i + 1];
      digit_groups[i] -= BASE;
      ++i;
    }
  } else {
    if (digit_groups.empty()) {
      digit_groups.emplace_back(1);
      positive ^= 1;
    } else {
      --digit_groups[0];
      size_t i = 0;
      while (digit_groups[i] < 0) {
        digit_groups[i] += BASE;
        --digit_groups[i + 1];
        ++i;
      }
    }
    fix_zero_digits();
  }
}

BigInteger& BigInteger::operator++() {
  add_one_with_sign(positive);
  return *this;
}

BigInteger& BigInteger::operator--() {
  add_one_with_sign(!positive);
  return *this;
}

BigInteger BigInteger::operator++(int) {
  BigInteger copy(*this);
  ++(*this);
  return copy;
}

BigInteger BigInteger::operator--(int) {
  BigInteger copy(*this);
  --(*this);
  return copy;
}

std::istream& operator>>(std::istream& in, BigInteger& x) {
  std::string s;
  in >> s;
  x = BigInteger(s);
  return in;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& x) {
  os << std::string(x);
  return os;
}

BigInteger operator""_bi(const char* buffer) {
  return BigInteger(std::string(buffer));
}

BigInteger abs(const BigInteger& a) { return BigInteger(a.digit_groups, true); }

BigInteger BigInteger::shift_right(size_t shift) const {
  if (shift >= digit_groups.size()) return BigInteger();
  return BigInteger(
      std::vector<long long>(digit_groups.begin() + shift, digit_groups.end()),
      positive);
}

BigInteger BigInteger::shift_left(size_t shift) const {
  std::vector<long long> new_digits(digit_groups.size() + shift);
  std::copy(digit_groups.begin(), digit_groups.end(),
            new_digits.begin() + shift);
  return BigInteger(std::move(new_digits), positive);
}

#include <iostream>

std::pair<BigInteger, BigInteger> BigInteger::divide21(const BigInteger& a,
                                                       const BigInteger& b,
                                                       size_t n) {
  if (a < b) {
    return {BigInteger(0), a};
  }
  if (a.digit_groups.size() <= SMALLDIVIDEDIGITS) {
    long long first(a);
    long long second(b);
    return {BigInteger(first / second), BigInteger(first % second)};
  }
  auto c = a.shift_right(n / 2);
  auto [coeff, rem] = divide32(c, b, n / 2);
  auto [coeff2, rem2] =
      divide32(rem.shift_left(n / 2) +
                   BigInteger(std::vector<long long>(
                                  a.digit_groups.begin(),
                                  a.digit_groups.begin() +
                                      std::min(n / 2, a.digit_groups.size())),
                              true),
               b, n / 2);
  return {coeff.shift_left(n / 2) + coeff2, rem2};
}

// takes only positive
std::pair<BigInteger, BigInteger> BigInteger::divide32(const BigInteger& a,
                                                       const BigInteger& b,
                                                       size_t n) {
  if (a < b) {
    return {BigInteger(0), a};
  }
  if (b.digit_groups.size() <= n) {
    return divide21(a, b, n);
  }
  if (a.digit_groups.size() <= SMALLDIVIDEDIGITS) {
    long long first(a);
    long long second(b);
    return {BigInteger(first / second), BigInteger(first % second)};
  }

  BigInteger coeff;
  size_t k = b.digit_groups.size() - n;
  auto a1 = a.shift_right(k);
  auto b1 = b.shift_right(k);

  if (b1.shift_left(n) < a1) {
    coeff = BigInteger(1).shift_left(n);
  } else {
    coeff = divide21(a1, b1, n).first;
  }
  auto current = a - b * coeff;
  while (current.is_negative()) {
    current += b;
    --coeff;
  }
  return {coeff, current};
}

std::pair<BigInteger, BigInteger> BigInteger::divide(const BigInteger& a,
                                                     const BigInteger& b) {
  if (compare_digit_groups(a.digit_groups, b.digit_groups) ==
      std::strong_ordering::less) {
    return {BigInteger(0), a};
  }
  size_t size = 1;
  while (size < std::max(a.digit_groups.size(), b.digit_groups.size())) {
    size *= 2;
  }
  auto [coeff, rem] = divide32(abs(a), abs(b), size);
  coeff.positive = a.positive == b.positive;
  rem.positive = a.positive;
  coeff.fix_zero_digits();
  rem.fix_zero_digits();
  return {coeff, rem};
}

BigInteger operator/(const BigInteger& a, const BigInteger& b) {
  return BigInteger::divide(a, b).first;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
  return *this = (*this / other);
}

long long BigInteger::inplace_small_divide(long long x) {
  long long carry = 0;
  for (size_t i = 0; i < digit_groups.size(); ++i) {
    size_t real_i = digit_groups.size() - i - 1;
    long long cur = digit_groups[real_i] + carry * BASE;
    digit_groups[real_i] = abs(cur / x);
    carry = cur % x;
  }
  return carry;
}

std::pair<BigInteger, long long> BigInteger::divide(const BigInteger& a, long long x) {
  bool x_pos = x >= 0;
  BigInteger copy(a);
  long long r = copy.inplace_small_divide(x);
  copy.positive = a.positive == x_pos;
  copy.fix_zero_digits();
  if (!a.positive) {
    r *= -1;
  }
  return make_pair(copy, r);
}

BigInteger& BigInteger::operator/=(long long x) {
  bool x_pos = x >= 0;
  inplace_small_divide(x);
  positive = positive == x_pos;
  fix_zero_digits();
  return *this;
}

BigInteger operator/(const BigInteger& a, long long b) {
  BigInteger copy(a);
  copy /= b;
  return copy;
}

BigInteger operator%(const BigInteger& a, const BigInteger& b) {
  return BigInteger::divide(a, b).second;
}

long long operator%(const BigInteger& a, long long b) {
  long long carry = 0;
  for (size_t i = 0; i < a.digit_groups.size(); ++i) {
    size_t real_i = a.digit_groups.size() - i - 1;
    long long cur = a.digit_groups[real_i] + carry * BigInteger::BASE;
    carry = cur % b;
  }
  return a.positive ? carry : -carry;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
  return *this = (*this % other);
}

BigInteger& BigInteger::operator%=(long long x) {
  long long res = inplace_small_divide(x);
  if (!positive) res *= -1;
  return (*this = res);
}

BigInteger operator-(const BigInteger& a) {
  return BigInteger(a.digit_groups, !a.positive);
}

std::vector<bool> BigInteger::ll_to_binary(long long x) {
  std::vector<bool> ans;
  while (x) {
    ans.emplace_back(x & 1ll);
    x >>= 1;
  }
  return ans;
}

void BigInteger::binary_add_to(std::vector<bool>& a,
                               const std::vector<bool>& b) {
  int carry = 0;
  a.resize(max(a.size(), b.size()));
  size_t i = 0;
  while (i < a.size() && (i < b.size() || carry)) {
    int cur = carry + static_cast<int>(a[i]) +
              (i < b.size() ? static_cast<int>(b[i]) : 0);
    a[i] = cur & 1;
    carry = cur >> 1;
    ++i;
  }
  if (i == a.size() && carry) {
    a.emplace_back(true);
  }
}

void BigInteger::binary_mul_to(std::vector<bool>& a,
                               const std::vector<bool>& b) {
  std::vector<long long> result_poly =
      FFT::multiply_poly<long long>(a.begin(), a.end(), b.begin(), b.end());
  for (size_t i = 0; i < result_poly.size(); ++i) {
    if (result_poly[i] > 1) {
      long long extra = result_poly[i] >> 1;
      result_poly[i] &= 1ll;
      if (i + 1 == result_poly.size()) {
        result_poly.emplace_back();
      }
      result_poly[i + 1] += extra;
    }
  }
  a.resize(result_poly.size());
  for (size_t i = 0; i < result_poly.size(); ++i) {
    a[i] = result_poly[i];
  }
}

std::vector<bool> BigInteger::to_binary() const {
  if (digit_groups.empty()) {
    return {};
  }
  size_t N = digit_groups.size();
  std::vector<std::vector<bool>> result(N);
  for (size_t i = 0; i < N; ++i) {
    result[i] = ll_to_binary(digit_groups[i]);
  }
  std::vector<bool> base_power = ll_to_binary(BASE);
  size_t step = 1;
  while (step < N) {
    for (size_t i = 0; i + step < N; i += 2 * step) {
      size_t next = i + step;
      binary_mul_to(result[next], base_power);
      binary_add_to(result[next], result[i]);
      std::swap(result[next], result[i]);
      result[next].clear();
    }
    step *= 2;
    binary_mul_to(base_power, base_power);
  }
  return result[0];
}

BigInteger BigInteger::power(BigInteger a, size_t n) {
  BigInteger ans = 1;
  // binary power
  while (n) {
    if (n & 1) {
      ans *= a;
    }
    n >>= 1;
    a *= a;
  }
  return ans;
}
