#ifndef NOYA_CHT_HPP
#define NOYA_CHT_HPP 1

#include "noya/limit.hpp"
#include <cassert>
#include <map>
#include <set>
#include <vector>

namespace noya {
template <class T> struct Linear_CHT_MIN {
  std::vector<std::pair<T, T>> stack;
  // make sure p is sorted by greater<>
  void add(T p, T q) {
    if (stack.size() >= 1) {
      assert(stack.back().first >= p);
    }
    if (stack.size() >= 1 && stack.back().first == p) {
      if (stack.back().second <= q) {
        return;
      } else {
        stack.pop_back();
      }
    }
    while ((int)stack.size() >= 2) {
      const auto [p2, q2] = stack.end()[-2];
      const auto [p1, q1] = stack.end()[-1];
      if ((__int128)(q1 - q2) * (p1 - p) < (__int128)(q - q1) * (p2 - p1)) {
        break;
      }
      stack.pop_back();
    }
    stack.emplace_back(p, q);
  }
  // make sure x is sorted by greater<>
  T get(T x) {
    while ((int)stack.size() >= 2) {
      const auto [p2, q2] = stack.end()[-2];
      const auto [p1, q1] = stack.end()[-1];
      if (p2 * x + q2 > p1 * x + q1) {
        break;
      } else {
        stack.pop_back();
      }
    }
    if (stack.empty()) {
      return infty<T>;
    } else {
      auto [k, b] = stack.back();
      return k * x + b;
    }
  }
};

// https://maspypy.github.io/library/convex/cht.hpp
template <typename T> struct Line {
  mutable T k, m, p;
  bool operator<(const Line &o) const { return k < o.k; }
  bool operator<(T x) const { return p < x; }
};

template <typename T> T lc_inf() { return std::numeric_limits<T>::max(); }
template <> long double lc_inf<long double>() { return 1 / .0; }

template <typename T> T lc_div(T a, T b) {
  return a / b - ((a ^ b) < 0 and a % b);
}
template <> long double lc_div(long double a, long double b) { return a / b; };
template <> double lc_div(double a, double b) { return a / b; };
template <typename T, bool MINIMIZE = true>
struct LineContainer : std::multiset<Line<T>, std::less<>> {
  using super = std::multiset<Line<T>, std::less<>>;
  using super::begin, super::end, super::insert, super::erase;
  using super::empty, super::lower_bound;
  T inf = lc_inf<T>();
  bool insect(typename super::iterator x, typename super::iterator y) {
    if (y == end())
      return x->p = inf, false;
    if (x->k == y->k)
      x->p = (x->m > y->m ? inf : -inf);
    else
      x->p = lc_div(y->m - x->m, x->k - y->k);
    return x->p >= y->p;
  }
  void add(T k, T m) {
    if (MINIMIZE) {
      k = -k, m = -m;
    }
    auto z = insert({k, m, 0}), y = z++, x = y;
    while (insect(y, z))
      z = erase(z);
    if (x != begin() and insect(--x, y))
      insect(x, y = erase(y));
    while ((y = x) != begin() and (--x)->p >= y->p)
      insect(x, erase(y));
  }
  T query(T x) {
    assert(!empty());
    auto l = *lower_bound(x);
    T v = (l.k * x + l.m);
    return (MINIMIZE ? -v : v);
  }
};

template <typename T> using CHT_min = LineContainer<T, true>;
template <typename T> using CHT_max = LineContainer<T, false>;
template <typename T> struct CHT_xy {
  static_assert(std::is_same_v<T, long long> || std::is_floating_point_v<T>);
  using ld = long double;
  CHT_min<ld> cht_min;
  CHT_max<ld> cht_max;
  T amax = -infty<T>, amin = infty<T>;
  T bmax = -infty<T>, bmin = infty<T>;
  int amax_idx = -1, amin_idx = -1;
  int bmax_idx = -1, bmin_idx = -1;
  bool empty = true;
  std::map<std::pair<T, T>, int> MP;

  void clear() {
    empty = true;
    cht_min.clear();
    cht_max.clear();
  }
  void add(T a, T b, int i = -1) {
    empty = false;
    cht_min.add(b, a);
    cht_max.add(b, a);
    std::pair<T, T> p = {a, b};
    MP[p] = i;

    if (amax < a)
      amax = a, amax_idx = i;
    if (amin > a)
      amin = a, amin_idx = i;
    if (bmax < b)
      bmax = b, bmax_idx = i;
    if (bmin > b)
      bmin = b, bmin_idx = i;
  }

  std::pair<T, int> get_max(T x, T y) {
    if (cht_min.empty())
      return {-infty<T>, -1};

    if (x == 0) {
      if (bmax * y > bmin * y) {
        return {bmax * y, bmax_idx};
      }
      return {bmin * y, bmin_idx};
    }
    ld z = ld(y) / x;
    if (x > 0) {
      auto l = cht_max.lower_bound(z);
      T a = l->m, b = l->k;
      std::pair<T, T> p = {a, b};
      int idx = MP[p];
      return {a * x + b * y, idx};
    }
    auto l = cht_min.lower_bound(z);
    T a = -(l->m), b = -(l->k);
    std::pair<T, T> p = {a, b};
    int idx = MP[p];
    return {a * x + b * y, idx};
  }

  std::pair<T, int> get_min(T x, T y) {
    auto [f, i] = get_max(-x, -y);
    return {-f, i};
  }
};
} // namespace noya

#endif // NOYA_CHT_HPP