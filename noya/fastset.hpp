#ifndef NOYA_FASTSET_HPP
#define NOYA_FASTSET_HPP 1
#include <assert.h>
#include <vector>
// https://github.com/hos-lyric/libra/blob/master/data_structure/fast_set.cpp
namespace noya {
struct Set {
  // max{ceil(log_64(n)), 1}
  int log64N, n;
  std::vector<unsigned long long> a[6];
  explicit Set(int n_ = 0) : n(n_) {
    assert(n >= 0);
    int m = n ? n : 1;
    for (int d = 0;; ++d) {
      m = (m + 63) >> 6;
      a[d].assign(m, 0);
      if (m == 1) {
        log64N = d + 1;
        break;
      }
    }
  }
  bool empty() const { return !a[log64N - 1][0]; }
  bool contains(int x) const { return (a[0][x >> 6] >> (x & 63)) & 1; }
  void insert(int x) {
    for (int d = 0; d < log64N; ++d) {
      const int q = x >> 6, r = x & 63;
      a[d][q] |= 1ULL << r;
      x = q;
    }
  }
  void erase(int x) {
    for (int d = 0; d < log64N; ++d) {
      const int q = x >> 6, r = x & 63;
      if ((a[d][q] &= ~(1ULL << r)))
        break;
      x = q;
    }
  }
  // max s.t. <= x (or -1)
  int prev(int x) const {
    if (x > n - 1)
      x = n - 1;
    for (int d = 0; d <= log64N; ++d) {
      if (x < 0)
        break;
      const int q = x >> 6, r = x & 63;
      const unsigned long long lower = a[d][q] << (63 - r);
      if (lower) {
        x -= __builtin_clzll(lower);
        for (int e = d; --e >= 0;)
          x = x << 6 | (63 - __builtin_clzll(a[e][x]));
        return x;
      }
      x = q - 1;
    }
    return -1;
  }
  // min s.t. >= x (or n)
  int next(int x) const {
    if (x < 0)
      x = 0;
    for (int d = 0; d < log64N; ++d) {
      const int q = x >> 6, r = x & 63;
      if (static_cast<unsigned>(q) >= a[d].size())
        break;
      const unsigned long long upper = a[d][q] >> r;
      if (upper) {
        x += __builtin_ctzll(upper);
        for (int e = d; --e >= 0;)
          x = x << 6 | __builtin_ctzll(a[e][x]);
        return x;
      }
      x = q + 1;
    }
    return n;
  }
};

template <class T> struct Painter {
  int n;
  Set s;
  std::vector<T> ts;
  Painter() {}
  Painter(int n_, const T &t) : n(n_), s(n + 1), ts(n + 2, t) {}
  template <class F> void paint(int a, int b, const T &t, F f) {
    assert(0 <= a);
    assert(a <= b);
    assert(b <= n);
    if (a == b)
      return;
    // auto it = this->lower_bound(a);
    int c = s.next(a);
    if (b < c) {
      f(a, b, ts[c]);
      s.insert(a);
      ts[a] = ts[c];
      s.insert(b);
      ts[b] = t;
    } else if (a < c) {
      const T ta = ts[c];
      int k = a;
      for (; c <= b; s.erase(c), c = s.next(c)) {
        f(k, c, ts[c]);
        k = c;
      }
      if (k < b) {
        f(k, b, ts[c]);
      }
      s.insert(a);
      ts[a] = ta;
      s.insert(b);
      ts[b] = t;
    } else {
      c = s.next(c + 1);
      int k = a;
      for (; c <= b; s.erase(c), c = s.next(c)) {
        f(k, c, ts[c]);
        k = c;
      }
      if (k < b) {
        f(k, b, ts[c]);
      }
      s.insert(b);
      ts[b] = t;
    }
  }
  void paint(int a, int b, const T &t) {
    paint(a, b, t, [&](int, int, const T &) -> void {});
  }
  T get(int k) const {
    assert(0 <= k);
    assert(k < n);
    return ts[s.next(k + 1)];
  }
};
} // namespace noya

#endif // NOYA_FASTSET_HPP