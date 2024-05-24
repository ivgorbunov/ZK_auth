#include "moduled_bigint.hpp"

ModuledBigInt::ModuledBigInt() {}

ModuledBigInt::ModuledBigInt(const BigInteger& val) : value(val) {
  fix_value();
}

ModuledBigInt::ModuledBigInt(long long val) : value(val) { fix_value(); }

void ModuledBigInt::fix_value() {
  if (!value.is_negative() && value < N) {
    return;
  }
  value %= N;
  if (value.is_negative()) {
    value += N;
  }
}

ModuledBigInt::ModuledBigInt(BigInteger&& val) : value(std::move(val)) {
  fix_value();
}

std::strong_ordering ModuledBigInt::operator<=>(
    const ModuledBigInt& other) const {
  return value <=> other.value;
}

ModuledBigInt& ModuledBigInt::operator+=(const ModuledBigInt& other) {
  value += other.value;
  if (value >= N) {
    value -= N;
  }
  return *this;
}

ModuledBigInt& ModuledBigInt::operator-=(const ModuledBigInt& other) {
  value -= other.value;
  if (value.is_negative()) {
    value += N;
  }
  return *this;
}

ModuledBigInt& ModuledBigInt::operator*=(const ModuledBigInt& other) {
  value *= other.value;
  value %= N;
  return *this;
}

ModuledBigInt operator+(const ModuledBigInt& a, const ModuledBigInt& b) {
  ModuledBigInt ans(a);
  ans += b;
  return ans;
}

ModuledBigInt operator-(const ModuledBigInt& a, const ModuledBigInt& b) {
  ModuledBigInt ans(a);
  ans -= b;
  return ans;
}

ModuledBigInt operator*(const ModuledBigInt& a, const ModuledBigInt& b) {
  ModuledBigInt ans(a);
  ans *= b;
  return ans;
}

ModuledBigInt operator-(const ModuledBigInt& a) {
  ModuledBigInt ans;
  ans -= a;
  return ans;
}

std::ostream& operator<<(std::ostream& os, const ModuledBigInt& a) {
  os << a.value;
  return os;
}

namespace {
// returns {x, y} such that ax+by=1 if a and b are coprime
// returns {0, 0} otherwise
std::pair<BigInteger, BigInteger> gcd_extended(BigInteger a, BigInteger b) {
  if (b.is_zero()) {
    if (a == BigInteger(1)) {
      return make_pair(BigInteger(1), BigInteger(0));
    }
    // not coprime
    return make_pair(BigInteger(), BigInteger());
  }
  auto [quot, rem] = BigInteger::divide(a, b);
  auto [x1, y1] = gcd_extended(b, rem);
  return make_pair(y1, x1 - y1 * quot);
}
};  // namespace

ModuledBigInt ModuledBigInt::inversed() const {

  return gcd_extended(value, ModuledBigInt::N).first;
}

const BigInteger& ModuledBigInt::get_value() const { return value; }

BigInteger ModuledBigInt::N = BigInteger(
    "27606985387162255149739023449107931668458716142620601169954803000803329");
