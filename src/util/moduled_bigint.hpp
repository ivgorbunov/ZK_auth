#pragma once

#include "bigint.hpp"
#include "N.hpp"

struct ModuledBigInt {
    ModuledBigInt() = default;

    ModuledBigInt(const BigInteger& value): value((value % N + N) % N) {
    }
    std::strong_ordering operator<=>(const ModuledBigInt& other) const = default;
    ModuledBigInt operator*=(const ModuledBigInt& other) {
        *this = ModuledBigInt{(value * other.value) % N};
        return *this;
    }

    ModuledBigInt operator-() {
        return ModuledBigInt{(N-value) % N};
    }
    BigInteger value;
};

ModuledBigInt operator+(ModuledBigInt a, ModuledBigInt b) {
    if (a.value + b.value >= N) {
        return ModuledBigInt{a.value + b.value - N};
    } else {
        return ModuledBigInt{a.value + b.value};
    }
}

ModuledBigInt operator-(ModuledBigInt a, ModuledBigInt b) {
    if (a.value >= b.value) {
        return ModuledBigInt{a.value - b.value};
    } else {
        return ModuledBigInt{N - b.value + a.value};
    }
}

ModuledBigInt operator*(const ModuledBigInt& a, ModuledBigInt b) {
    return ModuledBigInt{(a.value * b.value) % N};
}

// returns (x, y) such that ax+by=gcd(a,b)
pair<BigInteger, BigInteger> gcd_extended(BigInteger a, BigInteger b) {
    if (b == 0) {
        return make_pair(1, 0);
    }
    auto [quotient, remainder] = BigInteger::divide(a, b);
    auto [x1, y1] = gcd_extended(b, remainder);
    return make_pair(y1, x1 - quotient * y1);
}

ModuledBigInt GetInverse(const ModuledBigInt& a) {
    auto [x, y] = gcd_extended(a.value, N);
    return ModuledBigInt((x % N + N) % N);
}