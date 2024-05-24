#pragma once

#include <compare>

#include "bigint.hpp"

class ModuledBigInt {
 public:
  static BigInteger N;

  ModuledBigInt();
  ModuledBigInt(long long);
  ModuledBigInt(const BigInteger&);
  ModuledBigInt(BigInteger&&);
  ModuledBigInt(const ModuledBigInt&) = default;
  ModuledBigInt(ModuledBigInt&&) = default;

  ModuledBigInt& operator=(const ModuledBigInt&) = default;
  ModuledBigInt& operator=(ModuledBigInt&&) = default;

  std::strong_ordering operator<=>(const ModuledBigInt&) const;
  bool operator==(const ModuledBigInt&) const = default;
  bool operator!=(const ModuledBigInt&) const = default;

  ModuledBigInt& operator+=(const ModuledBigInt&);
  ModuledBigInt& operator-=(const ModuledBigInt&);
  ModuledBigInt& operator*=(const ModuledBigInt&);

  friend ModuledBigInt operator+(const ModuledBigInt&, const ModuledBigInt&);
  friend ModuledBigInt operator-(const ModuledBigInt&, const ModuledBigInt&);
  friend ModuledBigInt operator*(const ModuledBigInt&, const ModuledBigInt&);
  friend ModuledBigInt operator-(const ModuledBigInt&);

  // returns 0 if there is no inverse
  // when value and N are coprime
  ModuledBigInt inversed() const;

  friend std::ostream& operator<<(std::ostream&, const ModuledBigInt&);

  const BigInteger& get_value() const;

 private:
  void fix_value();

  BigInteger value;
};
