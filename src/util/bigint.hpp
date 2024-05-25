#pragma once

#include <algorithm>
#include <cmath>
#include <compare>
#include <string>
#include <vector>

using namespace std;

class BigInteger {
 public:
  BigInteger();
  explicit BigInteger(const std::string&);
  BigInteger(long long);
  BigInteger(const BigInteger&) = default;
  BigInteger(BigInteger&&);

  BigInteger& operator=(const BigInteger&) = default;
  BigInteger& operator=(BigInteger&&);

  explicit operator std::string() const;
  explicit operator bool() const;

  // may be unsafe if the number is too big!
  explicit operator long long() const;

  bool operator==(const BigInteger&) const = default;
  std::strong_ordering operator<=>(const BigInteger&) const;

  BigInteger& operator+=(const BigInteger&);
  BigInteger& operator-=(const BigInteger&);
  BigInteger& operator*=(const BigInteger&);
  BigInteger& operator/=(const BigInteger&);
  BigInteger& operator%=(const BigInteger&);
  BigInteger& operator/=(long long);
  BigInteger& operator%=(long long);

  // quotient, remainder
  static std::pair<BigInteger, BigInteger> divide(const BigInteger&,
                                                  const BigInteger&);
  static std::pair<BigInteger, long long> divide(const BigInteger&, long long);

  friend BigInteger operator+(const BigInteger&, const BigInteger&);
  friend BigInteger operator-(const BigInteger&, const BigInteger&);
  friend BigInteger operator*(const BigInteger&, const BigInteger&);
  friend BigInteger operator/(const BigInteger&, const BigInteger&);
  friend BigInteger operator%(const BigInteger&, const BigInteger&);
  friend BigInteger operator/(const BigInteger&, long long);
  friend long long operator%(const BigInteger&, long long);
  friend BigInteger operator-(const BigInteger&);

  BigInteger& operator++();
  BigInteger& operator--();
  BigInteger operator++(int);
  BigInteger operator--(int);

  friend std::istream& operator>>(std::istream&, BigInteger&);
  friend std::ostream& operator<<(std::ostream&, const BigInteger&);

  bool is_zero() const;
  bool is_negative() const;
  bool is_positive() const;

  friend BigInteger abs(const BigInteger&);

  // binary representation of absolute value
  // from least significant to most significant
  std::vector<bool> to_binary() const;

  friend BigInteger random_big_integer(BigInteger, BigInteger);
  static BigInteger power(BigInteger, size_t);


 private:
  void fix_zero_digits();
  static std::strong_ordering compare_digit_groups(
      const std::vector<long long>&, const std::vector<long long>&);
  void add_with_sign(bool, const std::vector<long long>&);
  BigInteger(const std::vector<long long>&, bool);
  BigInteger(std::vector<long long>&&, bool);
  void add_one_with_sign(bool);
  BigInteger shift_left(size_t) const;
  BigInteger shift_right(size_t) const;

  static std::pair<BigInteger, BigInteger> divide32(const BigInteger&,
                                                    const BigInteger&, size_t);
  static std::pair<BigInteger, BigInteger> divide21(const BigInteger&,
                                                    const BigInteger&, size_t);
  static const size_t SMALLDIVIDEDIGITS = 3;

  long long inplace_small_divide(long long);

  static std::vector<bool> ll_to_binary(long long);
  static void binary_mul_to(std::vector<bool>&, const std::vector<bool>&);
  static void binary_add_to(std::vector<bool>&, const std::vector<bool>&);

  static const long long BASE = 100'000;
  static const size_t BASELEN = 5;

  std::vector<long long> digit_groups;
  bool positive;
};

BigInteger operator""_bi(const char*);
