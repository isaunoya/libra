#ifndef NOYA_bitset_HPP
#define NOYA_bitset_HPP 1

#include "noya/limit.hpp"

#include <algorithm>
#include <cassert>
#include <ranges>
#include <string>
#include <vector>

namespace noya {

struct Bitset {
  int N, M;
  std::vector<ull> B;

  using T = noya::Bitset;
  static const ull U = -1ULL;

  Bitset(int n = 0) {
    N = n;
    M = (n + 63) >> 6;
    B.assign(M, 0);
  }

  int size() const { return N; }

  void norm() {
    if ((N & 63) != 0) {
      int a = N >> 6;
      int b = N & 63;
      B[a] &= (1ULL << (b + 1)) - 1;
    }
  }

  bool test(int i) const {
    assert(0 <= i && i < N);
    return B[i >> 6] & (1ULL << (i & 63));
  }

  void set() { B.assign(M, U), norm(); }

  void reset() { B.assign(M, 0); }

  template <class T> void set(int i, T value) {
    assert(0 <= i && i < N);
    if (value)
      B[i >> 6] |= 1ULL << (i & 63);
    else
      B[i >> 6] &= ~(1ULL << (i & 63));
  }
  void resize(int n) {
    N = n;
    M = (n + 63) >> 6;
    B.resize(M, 0);
  }
  bool empty() const {
    return std::ranges::all_of(B, [](ull x) { return x == 0; });
  }
  int count() const {
    int ans = 0;
    for (auto x : B) {
      ans += std::popcount(x);
    }
    return ans;
  }

  bool operator==(const T &rhs) const {
    return N == rhs.N && std::ranges::equal(B, rhs.B);
  }
  bool operator!=(const T &rhs) const { return !(*this == rhs); }

  T &operator=(const T &rhs) {
    if (this == &rhs)
      return *this;
    N = rhs.N;
    M = rhs.M;
    B = rhs.B;
    return *this;
  }

  T &operator|=(const T &rhs) {
    assert(N == rhs.N);
    for (int i = 0; i < M; ++i)
      B[i] |= rhs.B[i];
    return *this;
  }
  T &operator&=(const T &rhs) {
    assert(N == rhs.N);
    for (int i = 0; i < M; ++i)
      B[i] &= rhs.B[i];
    return *this;
  }
  T &operator^=(const T &rhs) {
    assert(N == rhs.N);
    for (int i = 0; i < M; ++i)
      B[i] ^= rhs.B[i];
    return *this;
  }

  T operator~() const {
    T result(N);
    for (int i = 0; i < M; ++i)
      result.B[i] = ~B[i];
    result.norm();
    return result;
  }

  T operator|(const T &rhs) const { return T(*this) |= rhs; }
  T operator&(const T &rhs) const { return T(*this) &= rhs; }
  T operator^(const T &rhs) const { return T(*this) ^= rhs; }

  std::vector<ull> get() { return std::vector<ull>(B.begin(), B.end()); }

  int _Find_first() {
    int x = 0;
    while (x < M) {
      if (B[x] == 0) {
        x += 1;
      } else {
        int y = __builtin_ctzll(B[x]);
        return x * 64 + y;
      }
    }
    return N;
  }

  // min s.t. > i (or N)
  int _Find_next(int i) {
    if (i == N)
      return N;
    i++;
    int a = i >> 6;
    int b = i & 63;
    if ((B[a] >> b) != 0) {
      int y = __builtin_ctzll(B[a] >> b);
      return a * 64 + b + y;
    }
    int x = a + 1;
    while (x < M) {
      if (B[x] == 0) {
        x += 1;
      } else {
        int y = __builtin_ctzll(B[x]);
        return x * 64 + y;
      }
    }
    return N;
  }

  std::vector<int> getOnes() {
    std::vector<int> v;
    for (int i = _Find_first(); i < N; i = _Find_next(i))
      v.push_back(i);
    return v;
  }

  std::vector<int> getZeros() {
    std::vector<int> v = getOnes();
    v.push_back(N);
    std::vector<int> zeros;
    int last = 0;
    for (auto pos : v) {
      for (int i = last; i < pos; i++)
        zeros.push_back(i);
      last = pos + 1;
    }
    return zeros;
  }

  ull start_u64(int i) {
    assert(0 <= i && i < N);
    int x = i & 63;
    int b = i >> 6;
    if (x == 0)
      return B[b];

    ull left = B[b] >> x;
    if (b + 1 == M)
      return left;

    assert(b + 1 < M);
    ull right = B[b + 1] << (64 - x);
    return left | right;
  }

  T prod(int l, int r) {
    assert(0 <= l && l <= r && r <= N);

    if (l == r)
      return T();

    int len = r - l;
    T result(len);

    int m = result.M;
    for (int i = 0; i < m; i++) {
      result.B[i] = start_u64(l + i * 64);
    }
    result.norm();
    return result;
  }

  template <class T> void operator[](int i, int value) {
    assert(0 <= i && i < N);
    set(i, value);
  }

  class reference {
  public:
    reference(T &b, int idx) : T_(b), index_(idx) {}

    reference &operator=(bool value) {
      T_.set(index_, value);
      return *this;
    }

    reference &operator=(const reference &other) { return *this = bool(other); }

    operator bool() const { return T_.test(index_); }

  private:
    T &T_;
    int index_;
  };

  reference operator[](int i) {
    assert(0 <= i && i < N);
    return reference(*this, i);
  }
};

std::string to_string(Bitset B) {
  int N = B.N;
  std::string ans = "";
  for (int i = 0; i < N; i++)
    ans += B[i] ? '1' : '0';
  return ans;
}

} // namespace noya

#endif // NOYA_bitset_HPP