#ifndef NOYA_ROLLINGHASH_HPP
#define NOYA_ROLLINGHASH_HPP 1

#include <atcoder/modint.hpp>
#include <noya/rnd.hpp>
#include <vector>

namespace noya {
static int64_t base_t = internal::gen_values();
template <class T, class S> struct hash_array {
  int n;
  std::vector<T> H;
  std::vector<T> pw;
  const T base = T(base_t);
  hash_array(const std::vector<S> &a) { build(a); }
  hash_array(const std::string &a) {
    std::vector<S> a0(a.begin(), a.end());
    build(a0);
  }
  hash_array() {}

  void build(const std::vector<S> &a) {
    n = int(a.size());
    H.resize(n + 1);
    for (int i = 0; i < n; i++) {
      H[i + 1] = H[i] * base + T(a[i]);
    }
    pw.resize(n + 1);
    pw[0] = 1;
    for (int i = 0; i < n; i++) {
      pw[i + 1] = pw[i] * base;
    }
  }
  void build(const std::string &a) {
    std::vector<S> a0(a.begin(), a.end());
    build(a0);
  }

  // [l, r)
  T prod(int l, int r) const {
    assert(0 <= l && l <= r && r <= n);
    T X = H[r] - H[l] * pw[r - l];
    return X;
  }
};

template <class A, class B, class S> struct double_hash_array {
  int n;

  hash_array<A, S> A_hash_array;
  hash_array<B, S> B_hash_array;

  double_hash_array(const std::vector<S> &a) { build(a); }
  double_hash_array() {}
  double_hash_array(const std::string &a) {
    std::vector<S> a0(a.begin(), a.end());
    build(a0);
  }

  void build(const std::vector<S> &a) {
    n = int(a.size());
    A_hash_array.build(a);
    B_hash_array.build(a);
  }
  void build(const std::string &a) {
    std::vector<S> a0(a.begin(), a.end());
    build(a0);
  }

  std::pair<A, B> prod(int l, int r) const {
    return std::make_pair(A_hash_array.prod(l, r), B_hash_array.prod(l, r));
  }
};
}; // namespace noya

using mint1 = atcoder::modint1000000007;
using mint2 = atcoder::modint998244353;
using mint3 = uint64_t;

template <class T> int lcp(T &A, int i, T &B, int j) {
  int n = A.n;
  int m = B.n;

  int l = 0;
  int r = std::min(n - i, m - j) + 1;

  while (r - l > 1) {
    int m = (l + r) / 2;
    if (A.prod(i, i + m) == B.prod(j, j + m))
      l = m;
    else
      r = m;
  }
  return l;
}

template <class T> int lcs(T &A, int i, T &B, int j) {
  int l = 0;
  int r = std::min(i + 1, j + 1) + 1;

  while (r - l > 1) {
    int m = (l + r) / 2;
    if (A.prod(i + 1 - m, i + 1) == B.prod(j + 1 - m, j + 1))
      l = m;
    else
      r = m;
  }
  return l;
}

template <class T> bool same(T &A, int l1, int r1, T &B, int l2, int r2) {
  int n1 = r1 - l1;
  int n2 = r2 - l2;
  
  assert(0 <= l1 && l1 <= r1 && r1 <= n1);
  assert(0 <= l2 && l2 <= r2 && r2 <= n2);

  if (n1 != n2)
    return false;
  int lc = lcp(A, l1, B, l2);
  if (lc == n1)
    return true;
  else
    return false;
}

#endif // NOYA_ROLLINGHASH_HPP