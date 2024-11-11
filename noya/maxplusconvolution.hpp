#ifndef NOYA_MAXPLUS_CONVOLUTION_HPP
#define NOYA_MAXPLUS_CONVOLUTION_HPP 1

#include "noya/smawk.hpp"

#include "functional"
#include "vector"

template <class T>
std::vector<T> concave_maxplus_convolution(const std::vector<T> &a,
                                           const std::vector<T> &b) {
  const int n = int(a.size());
  const int m = int(b.size());
  const auto get = [&](const int &i, const int &j) -> T {
    return a[j] + b[i - j];
  };
  const auto select = [&](const int &i, const int &j, const int &k) -> bool {
    if (i < k)
      return false;
    if (i - j >= m)
      return true;
    return get(i, j) <= get(i, k);
  };
  const auto amax = smawk(n + m - 1, n, select);
  std::vector<T> c(n + m - 1);
  for (int i = 0; i < n + m - 1; i++)
    c[i] = get(i, amax[i]);
  return c;
}

template <class T>
std::vector<T> concave_minplus_convolution(const std::vector<T> &a,
                                           const std::vector<T> &b) {
  const int n = int(a.size());
  const int m = int(b.size());
  std::vector<T> negative_a(n);
  std::vector<T> negative_b(m);
  for (int i = 0; i < n; i++) {
    negative_a[i] = -a[i];
  }
  for (int i = 0; i < m; i++) {
    negative_b[i] = -b[i];
  }
  auto negative_c = concave_maxplus_convolution(negative_a, negative_b);
  std::vector<T> c(n + m - 1);
  for (int i = 0; i < n + m - 1; i++)
    c[i] = -negative_c[i];
  return c;
}

#endif // NOYA_MAXPLUS_CONVOLUTION_HPP