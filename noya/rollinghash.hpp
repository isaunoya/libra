#ifndef NOYA_ROLLINGHASH_HPP
#define NOYA_ROLLINGHASH_HPP 1

#include <atcoder/modint.hpp>
#include <noya/rnd.hpp>
#include <vector>

namespace noya {
template <class T, class S> struct hash_array {
  int n;
  std::vector<T> H;
  std::vector<T> pw;
  T base = T(internal::gen_values());
  hash_array(std::vector<S> &a) { build(a); }
  hash_array() {}

  void build(std::vector<S> &a) {
    n = (int)a.size();
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

  // [l, r)
  T prod(int l, int r) {
    assert(0 <= l && l <= r && r <= n);
    T X = H[r] - H[l] * pw[r - l];
    return X;
  }
};

template <class A, class B, class S> struct double_hash_array {
  int n;

  hash_array<A, S> A_hash_array;
  hash_array<B, S> B_hash_array;
  double_hash_array(std::vector<S> &a) { build(a); }
  double_hash_array() {}

  void build(std::vector<S> &a) {
    A_hash_array.build(a);
    B_hash_array.build(a);
  }

  std::pair<A, B> prod(int l, int r) {
    return make_pair(A_hash_array.prod(l, r), B_hash_array.prod(l, r));
  }
};

}; // namespace noya

#endif // NOYA_ROLLINGHASH_HPP