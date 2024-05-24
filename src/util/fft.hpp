#pragma once

#include <vector>

namespace FFT {
template <typename T>
struct complex {
  T x, y;

  complex() : x(0), y(0) {}
  complex(T x, T y) : x(x), y(y) {}

  complex operator*(const complex &c) const {
    return complex(x * c.x - y * c.y, x * c.y + y * c.x);
  }

  complex operator+(const complex &c) const {
    return complex(x + c.x, y + c.y);
  }

  complex operator-(const complex &c) const {
    return complex(x - c.x, y - c.y);
  }

  template <typename value_t>
  complex operator/(const value_t &value) const {
    return complex(x / value, y / value);
  }

  complex conj() const { return complex(x, -y); }
};

template <typename float_t = long double>
void fft(std::vector<complex<float_t>> &a) {
  static std::vector<int> reversed_mask;
  static std::vector<complex<float_t>> roots = std::vector<complex<float_t>>(1);
  static constexpr float_t PI = M_PI;

  if (a.empty()) return;

  int n = int(a.size());
  // assert((n & (n - 1)) == 0);

  if (int(reversed_mask.size()) != n) {
    int lg = 0;
    {
      auto copy = n;
      while (copy > 1) {
        lg++;
        copy /= 2;
      }
    }
    //__lg(n)
    reversed_mask.resize(n);
    for (int mask = 1; mask < n; mask++)
      reversed_mask[mask] =
          (reversed_mask[mask >> 1] >> 1) + ((mask & 1) << (lg - 1));
  }

  if (int(roots.size()) < n) {
    int prev_size = roots.size();
    roots.resize(n);
    for (int len = prev_size; len < n; len <<= 1)
      for (int i = 0; i < len; i++)
        roots[len + i] =
            complex<float_t>(cosl(PI * i / len), sinl(PI * i / len));
  }

  auto cp = reversed_mask;
  for (int i = 0; i < n; i++) {
    if (i < reversed_mask[i]) std::swap(a[i], a[reversed_mask[i]]);
  }

  for (int len = 1; len < n; len <<= 1)
    for (int i = 0; i < n; i += (len << 1))
      for (int j = 0; j < len; j++) {
        complex<float_t> value = a[i + j + len] * roots[len + j];
        a[i + j + len] = a[i + j] - value;
        a[i + j] = a[i + j] + value;
      }
}

template <typename result_t, typename float_t = long double, typename T1,
          typename T2>
std::vector<result_t> multiply_poly(T1 a_begin, T1 a_end, T2 b_begin,
                                    T2 b_end) {
  static constexpr float_t PI = M_PI;

  if (a_begin == a_end || b_begin == b_end) return {};

  static constexpr int BUBEN = 20;
  int n = std::distance(a_begin, a_end);
  int m = std::distance(b_begin, b_end);
  if (std::min(n, m) <= BUBEN) {
    vector<result_t> product(n + m - 1);
    for (int i = 0; a_begin != a_end; i++, a_begin++) {
      T2 iterator = b_begin;
      for (int j = 0; iterator != b_end; j++, iterator++)
        product[i + j] += result_t(*a_begin) * result_t(*iterator);
    }
    return product;
  }

  int real_size = n + m - 1;
  int base = 2;
  while (base < real_size) base <<= 1;

  std::vector<complex<float_t>> res(base);
  for (int i = 0; a_begin != a_end; i++, a_begin++) res[i].x = *a_begin;

  for (int i = 0; b_begin != b_end; i++, b_begin++) res[i].y = *b_begin;

  fft<float_t>(res);
  complex<float_t> coeff(0, float_t(-0.25) / base);
  for (int i = 0; i <= (base >> 1); i++) {
    int j = (base - i) & (base - 1);
    complex<float_t> num = (res[j] * res[j] - (res[i] * res[i]).conj()) * coeff;
    res[j] = (res[i] * res[i] - (res[j] * res[j]).conj()) * coeff;
    res[i] = num;
  }
  // fft(product) == res

  for (int i = 0; i < (base >> 1); i++) {
    complex<float_t> a0 = (res[i] + res[i + (base >> 1)]);
    complex<float_t> a1 = (res[i] - res[i + (base >> 1)]) *
                          complex<float_t>(cosl(PI * i / (base >> 1)),
                                           sinl(PI * i / (base >> 1)));
    res[i] = a0 + a1 * complex<float_t>(0, 1);
  }

  res.resize(base >> 1);
  fft<float_t>(res);
  std::vector<result_t> product(real_size);

  for (int i = 0; i < real_size; i++)
    product[i] = ((i & 1) ? res[i >> 1].y : res[i >> 1].x) +
                 (std::is_integral<result_t>::value) * float_t(0.5);

  return product;
}

template <typename T>
std::vector<T> normalize(std::vector<T> pol) {
  while (!pol.empty() && pol.back() == 0) pol.pop_back();

  return pol;
}

}  // namespace FFT
