#include <iostream>
#include <compare>
#include <string>
#include <complex>
#include <vector>
#include <cmath>

#define double long double

enum class SignT {
    NEGATIVE = -1,
    ZERO     =  0,
    POSITIVE =  1,
};

SignT reverse(SignT a) {
    switch (a) {
        case SignT::NEGATIVE:
            return SignT::POSITIVE;
        case SignT::ZERO:
            return SignT::ZERO;
        case SignT::POSITIVE:
            return SignT::NEGATIVE;
    }
}

void fft(std::vector<std::complex<double>> &a, bool inverted) {
    int n = a.size();
    int k = 0;
    while ((1 << k) < n) {
        k++;
    }
    for (int i = 0; i < n; i++) {
        int rev_i = 0;
        for (int j = 0; j < k; j++) {
            if (i & (1ll << j)) {
                rev_i |= (1 << (k - j - 1));
            }
        }
        if (i < rev_i) {
            swap(a[i], a[rev_i]);
        }
    }
    for (int len = 2; len <= n; len *= 2) {
        double ang = 2.0 * M_PI / len * (inverted ? -1 : 1);
        std::complex c_len(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            std::complex<double> c(1);
            for (int j = 0; j < len / 2; ++j) {
                std::complex<double> u = a[i + j],  v = a[i + j + len / 2] * c;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                c *= c_len;
            }
        }
    }
    if (inverted) {
        for (int i = 0; i < n; i++) {
            a[i] /= n;
        }
    }
}

class BigInteger {
private:
    SignT sign = SignT::ZERO;
    std::vector<int> digits;

    size_t size() const {
        return digits.size();
    }

    static const int BASE = 10000;
    static const size_t BASE_LEN = 4;
public:
    BigInteger() = default;

    BigInteger(long long value) {
        if (value == 0) {
            sign = SignT::ZERO;
            return;
        }
        if (value < 0) {
            sign = SignT::NEGATIVE;
            value *= -1;
        } else {
            sign = SignT::POSITIVE;
        }
        while (value != 0) {
            digits.push_back(value % BASE);
            value /= BASE;
        }
    }

    BigInteger(std::string& s) {
        if (s == "0") {
            return;
        }
        size_t start = 0;
        if (s[0] == '-') {
            start++;
            sign = SignT::NEGATIVE;
        } else {
            sign = SignT::POSITIVE;
        }
        int now_val = 0;
        int now_10 = 1;
        size_t now_cnt = 0;
        for (size_t i = s.size(); i-- > start;) {
            now_val += (s[i] - '0') * now_10;
            now_10 *= 10;
            now_cnt++;
            if (now_cnt == BASE_LEN) {
                digits.push_back(now_val);
                now_val = 0;
                now_cnt = 0;
                now_10 = 1;
            }
        }
        if (now_cnt) {
            digits.push_back(now_val);
        }
    }

    BigInteger(const BigInteger& another): sign(another.sign), digits(another.digits) {}

    BigInteger& operator=(const BigInteger& another) {
        sign = another.sign;
        digits = another.digits;
        return *this;
    }

    std::weak_ordering operator<=>(const BigInteger& another) const {
        if (sign != another.sign) {
            return sign <=> another.sign;
        }
        if (sign == SignT::ZERO) {
            return std::weak_ordering::equivalent;
        }
        if (digits.size() != another.digits.size()) {
            if (sign == SignT::POSITIVE) {
                return digits.size() <=> another.digits.size();
            } else {
                return another.digits.size() <=> digits.size();
            }
        }
        for (size_t i = digits.size() - 1; i >= 0; --i) {
            if (digits[i] != another.digits[i]) {
                if (sign == SignT::POSITIVE) {
                    return digits[i] <=> another.digits[i];
                } else {
                    return another.digits[i] <=> digits[i];
                }
            }
            if (i == 0) {
                break;
            }
        }
        return std::weak_ordering::equivalent;
    }

    bool operator==(const BigInteger& another) const {
        return sign == another.sign && digits.size() == another.size() && digits == another.digits;
    }

    bool operator!=(const BigInteger& another) const {
        return !(*this == another);
    }

    BigInteger operator-() const {
        BigInteger ans = *this;
        ans.sign = reverse(ans.sign);
        return ans;
    }

    BigInteger& operator+=(const BigInteger& another) {
        if (*this == -another) {
            digits.clear();
            sign = SignT::ZERO;
            return *this;
        }
        bool is_ans_negative = (*this < -another);
        digits.resize(std::max(digits.size(), another.size()) + 1, 0);
        int carry = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            int now = digits[i] * static_cast<int>(sign);
            if (i < another.size()) {
                now += another.digits[i] * static_cast<int>(another.sign);
            } else {
                if (carry == 0) {
                    break;
                }
            }
            if (is_ans_negative) {
                now *= -1;
            }
            digits[i] = now + carry;
            carry = 0;
            if (digits[i] < 0) {
                digits[i] += BASE;
                carry = -1;
            }
            if (digits[i] >= BASE) {
                digits[i] -= BASE;
                carry = 1;
            }
        }
        if (is_ans_negative) {
            sign = SignT::NEGATIVE;
        } else {
            sign = SignT::POSITIVE;
        }
        while (digits.back() == 0) {
            digits.pop_back();
        }
        return *this;
    }

    BigInteger& operator-=(const BigInteger& another) {
        return (*this += (-another));
    }


    BigInteger& operator*=(const BigInteger& another) {
        if (sign == SignT::ZERO || another.sign == SignT::ZERO) {
            return *this = BigInteger(0);
        }
        size_t len = 1;
        while (len < std::max(size(), another.size())) {
            len *= 2;
        }
        std::vector<std::complex<double>> v1(len * 2);
        std::vector<std::complex<double>> v2(len * 2);
        for (size_t i = 0; i < digits.size(); i++) {
            v1[i] = digits[i];
        }
        for (size_t i = 0; i < another.size(); i++) {
            v2[i] = another.digits[i];
        }
        fft(v1, 0);
        fft(v2, 0);
        for (size_t i = 0; i < len; ++i) {
            v1[i] *= v2[i];
        }
        fft(v1, 1);
        sign = (sign == another.sign) ? SignT::POSITIVE : SignT::NEGATIVE;
        digits.clear();
        long long carry = 0;
        for (size_t i = 0; i < len; ++i) {
            long long now = static_cast<long long>(v1[i].real() + 0.5) + carry;
            digits.push_back(now % BASE);
            carry = now / BASE;
        }
        while (digits.size() && digits.back() == 0) {
            digits.pop_back();
        }
        return *this;
    }

    std::pair<BigInteger, BigInteger> div(const BigInteger& another) const {
        if (sign == SignT::ZERO) {
            return std::make_pair(*this, *this);
        }
        BigInteger now = 0;
        std::string ans_s;
        std::string s = (another.abs()).toString();
        for (size_t j = 0; j < size() * BASE_LEN; ++j) {
            s.push_back('0');
        }
        for (size_t i = 0; i <= size() * BASE_LEN; ++i) {
            BigInteger to_add(s);
            int ans_val = 0;
            while (now <= (*this).abs()) {
                now += to_add;
                ans_val++;
            }
            now -= to_add;
            ans_val--;
            ans_s.push_back(ans_val + '0');
            s.pop_back();
        }
        reverse(ans_s.begin(), ans_s.end());
        while (ans_s.size() && ans_s.back() == '0') {
            ans_s.pop_back();
        }
        reverse(ans_s.begin(), ans_s.end());
        BigInteger ans_div(ans_s);
        if (ans_div.digits.size() == 0) {
            ans_div.sign = SignT::ZERO;
        } else {
            if (sign == another.sign) {
                ans_div.sign = SignT::POSITIVE;
            } else {
                ans_div.sign = SignT::NEGATIVE;
            }
        }
        BigInteger ans_mod = (*this).abs();
        ans_mod -= now;
        if (ans_mod.sign != SignT::ZERO) {
            ans_mod.sign = sign;
        }
        return std::make_pair(ans_div, ans_mod);
    }

    BigInteger& operator/=(const BigInteger& another) {
        *this = div(another).first;
        return *this;
    }

    BigInteger& operator%=(const BigInteger& another) {
        *this = div(another).second;
        return *this;
    }

    BigInteger& operator++() {
        *this += 1;
        return *this;
    }

    BigInteger& operator--() {
        *this -= 1;
        return *this;
    }

    BigInteger& operator++(int) {
        *this += 1;
        return *this;
    }

    BigInteger& operator--(int) {
        *this -= 1;
        return *this;
    }

    BigInteger abs() const {
        BigInteger ans = *this;
        if (ans.sign == SignT::NEGATIVE) {
            ans.sign = SignT::POSITIVE;
        }
        return ans;
    }

    std::string toString() const {
        if (sign == SignT::ZERO) {
            return "0";
        }

        std::string ans;
        if (sign == SignT::NEGATIVE) {
            ans.push_back('-');
        }
        for (size_t i = digits.size(); i--;) {
            int val = digits[i];
            std::string now;
            for (size_t j = 0; j < BASE_LEN; j++) {
                now.push_back(val % 10 + '0');
                val /= 10;
            }

            while (i + 1 == digits.size() && now.back() == '0') {
                now.pop_back();
            }
            reverse(now.begin(), now.end());
            ans += now;
        }
        return ans;
    }



    explicit operator bool() const {
        return (sign != SignT::ZERO);
    }
};


BigInteger operator+(const BigInteger& first, const BigInteger& second) {
    BigInteger ans = first;
    ans += second;
    return ans;
}

BigInteger operator-(const BigInteger& first, const BigInteger& second) {
    BigInteger ans = first;
    ans -= second;
    return ans;
}

BigInteger operator*(const BigInteger& first, const BigInteger& second) {
    BigInteger ans = first;
    ans *= second;
    return ans;
}

BigInteger operator/(const BigInteger& first, const BigInteger& second) {
    BigInteger ans = first;
    ans /= second;
    return ans;
}

BigInteger operator%(const BigInteger& first, const BigInteger& second) {
    BigInteger ans = first;
    ans %= second;
    return ans;
}

std::istream& operator>> (std::istream& in, BigInteger& b) {
    std::string s;
    in >> s;
    b = BigInteger(s);
    return in;
}

std::ostream& operator<< (std::ostream& out, const BigInteger& b) {
    std::string s = b.toString();
    out << s;
    return out;
}

BigInteger operator"" _bi(unsigned long long another) {
    BigInteger ans(another);
    return ans;
}

BigInteger gcd(BigInteger a, BigInteger b) {
    if (!static_cast<bool>(b)) {
        return a;
    } else {
        return gcd(b, a % b);
    }
}

class Rational {
private:
    BigInteger numenator, denumenator;

    void fix() {
        if (numenator == 0_bi) {
            denumenator = 1;
            return;
        }
        if (denumenator < 0_bi) {
            denumenator *= -1;
            numenator *= -1;
        }
        BigInteger s = gcd(numenator.abs(), denumenator);
        numenator /= s;
        denumenator /= s;
    }
public:
    Rational(): numenator(0), denumenator(1) {};

    Rational(int val): numenator(val), denumenator(1) {}

    Rational(const BigInteger& val): numenator(val), denumenator(1) {}

    std::weak_ordering operator<=> (const Rational& another) {
        return (numenator * another.denumenator) <=> (denumenator * another.numenator);
    }

    bool operator== (const Rational& another) const = default;
    bool operator!= (const Rational& another) const = default;

    Rational operator-() const {
        Rational ans = *this;
        ans.numenator *= -1;
        return ans;
    }

    Rational& operator+= (const Rational& another) {
        numenator *= another.denumenator;
        numenator += another.numenator * denumenator;
        denumenator *= another.denumenator;
        fix();
        return *this;
    }

    Rational& operator-= (const Rational& another) {
        return (*this += (-another));
    }

    Rational& operator*= (const Rational& another) {
        numenator *= another.numenator;
        denumenator *= another.denumenator;
        fix();
        return *this;
    }

    Rational& operator/= (const Rational& another) {
        numenator *= another.denumenator;
        denumenator *= another.numenator;
        fix();
        return *this;
    }

    std::string toString() const {

        std::string ans = numenator.toString();
        if (denumenator != 1_bi) {
            ans.push_back('/');
            ans += denumenator.toString();
        }
        return ans;
    }

    std::string asDecimal(size_t precision = 0) const {
        BigInteger now = 1_bi;
        for (size_t i = 0; i < precision; ++i) {
            now *= 10_bi;
        }
        std::string ans1 = ((now * numenator.abs()) / denumenator).toString();
        std::string ans;

        if (numenator < 0) {
            ans.push_back('-');
        }


        if (ans1.size() <= precision) {
            ans.push_back('0');
        } else {
            for (size_t i = 0; i < ans1.size() - precision; ++i) {
                ans += ans1[i];
            }
        }
        if (precision) {
            ans += '.';
        }
        for (size_t i = 0; i < precision; ++i) {
            if (ans1.size() + i >= precision) {
                ans += ans1[ans1.size() - precision + i];
            } else {
                ans += '0';
            }
        }
        return ans;
    }

    explicit operator double() const {
        std::string s = asDecimal(10);
        double ans = stod(s);
        return ans;
    }
};

Rational operator+(const Rational& first, const Rational& second) {
    Rational ans = first;
    ans += second;
    return ans;
}

Rational operator-(const Rational& first, const Rational& second) {
    Rational ans = first;
    ans -= second;
    return ans;
}

Rational operator*(const Rational& first, const Rational& second) {
    Rational ans = first;
    ans *= second;
    return ans;
}

Rational operator/(const Rational& first, const Rational& second) {
    Rational ans = first;
    ans /= second;
    return ans;
}
