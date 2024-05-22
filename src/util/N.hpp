#include "bigint.hpp"

BigInteger P = (BigInteger("85070591730234615865843651857942052864") * BigInteger(2)) - BigInteger(1);
BigInteger Q = (BigInteger("81129638414606681695789005144064") * BigInteger(2)) - BigInteger(1);
BigInteger N = P * Q;
