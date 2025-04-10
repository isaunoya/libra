#ifndef NOYA_CONVOLUTION_HPP
#define NOYA_CONVOLUTION_HPP 1

#include "atcoder/convolution.hpp"

namespace noya {
static int highest_bit(unsigned x) {
  return x == 0 ? -1 : 31 - __builtin_clz(x);
}
template <class T>
std::vector<T> bitwise_and_convolution(std::vector<T> a, std::vector<T> b) {
  int N = int(a.size());
  assert(int(b.size()) == N);
  int e = highest_bit(N);

  assert(N == (1 << e));

  auto convolution = [&](std::vector<T> &f) -> void {
    for (int j = 0; j < e; j++)
      for (int i = 0; i < N; i++)
        if (i >> j & 1)
          f[i ^ 1 << j] += f[i];
  };
  auto envolution = [&](std::vector<T> &f) -> void {
    for (int j = 0; j < e; j++)
      for (int i = 0; i < N; i++)
        if (i >> j & 1)
          f[i ^ 1 << j] -= f[i];
  };

  convolution(a), convolution(b);
  std::vector<T> c(N);
  for (int i = 0; i < N; i++)
    c[i] = a[i] * b[i];
  envolution(c);
  return c;
}
template <class T>
std::vector<T> bitwise_or_convolution(std::vector<T> a, std::vector<T> b) {
  int N = int(a.size());
  assert(int(b.size()) == N);
  int e = highest_bit(N);

  assert(N == (1 << e));

  std::reverse(a.begin(), a.end());
  std::reverse(b.begin(), b.end());
  auto c = bitwise_and_convolution(a, b);
  std::reverse(c.begin(), c.end());
  return c;
}

template <class T>
std::vector<T> bitwise_xor_convolution(std::vector<T> a, std::vector<T> b) {
  int N = int(a.size());
  assert(int(b.size()) == N);
  int e = highest_bit(N);

  assert(N == (1 << e));

  auto convolution = [&](std::vector<T> &f) -> void {
    for (int j = 0; j < e; j++)
      for (int i = 0; i < N; i++)
        if (i >> j & 1) {
          T x = f[i], y = f[i ^ (1 << j)];
          f[i] = y - x;
          f[i ^ (1 << j)] = x + y;
        }
  };
  auto envolution = [&](std::vector<T> &f) -> void {
    convolution(f);
    T inv = T(N).inv();
    for (int i = 0; i < N; i++)
      f[i] *= inv;
  };

  convolution(a), convolution(b);
  std::vector<T> c(N);
  for (int i = 0; i < N; i++)
    c[i] = a[i] * b[i];
  envolution(c);
  return c;
}

template <class T>
std::vector<T> subset_convolution(std::vector<T> a, std::vector<T> b) {
  int N = int(a.size());
  assert(int(b.size()) == N);
  int e = highest_bit(N);

  assert(N == (1 << e));
  auto convolution = [&](std::vector<T> &f) -> void {
    for (int j = 0; j < e; j++)
      for (int i = 0; i < N; i++)
        if (~i >> j & 1)
          f[i ^ 1 << j] += f[i];
  };
  auto envolution = [&](std::vector<T> &f) -> void {
    for (int j = 0; j < e; j++)
      for (int i = 0; i < N; i++)
        if (~i >> j & 1)
          f[i ^ 1 << j] -= f[i];
  };

  std::vector<std::vector<T>> f(e + 1, std::vector<T>(N));
  std::vector<std::vector<T>> g(e + 1, std::vector<T>(N));
  std::vector<T> ans(N);

  for (int i = 0; i < N; i++) {
    f[__builtin_popcount(i)][i] = a[i];
    g[__builtin_popcount(i)][i] = b[i];
  }

  for (int i = 0; i < e; i++) {
    convolution(f[i]);
    convolution(g[i]);
  }

  std::vector<T> tmp(N);
  for (int i = 0; i <= e; i++) {
    for (int j = 0; j < N; j++)
      tmp[j] = 0;
    for (int j = 0; j <= i; j++)
      for (int k = 0; k < N; k++)
        tmp[k] += f[j][k] * g[i - j][k];
    envolution(tmp);
    for (int k = 0; k < N; k++)
      if (__builtin_popcount(k) == i)
        ans[k] = tmp[k];
  }
  return ans;
}
} // namespace noya

#endif // NOYA_CONVOLUTION_HPP