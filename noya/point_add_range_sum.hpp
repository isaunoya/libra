#ifndef NOYA_POINT_ADD_RANGE_SUM_HPP
#define NOYA_POINT_ADD_RANGE_SUM_HPP 1

#include "atcoder/fenwicktree.hpp"
#include <cmath>
#include <vector>

namespace noya {
template <class T> struct block {
  int V, sqrtV;

  block() {}
  block(const int &_V) {
    if (_V > 0) {
      build(_V);
    }
  }

  std::vector<T> point, blo;
  void build(const int &_V) {
    V = _V;
    sqrtV = sqrt(V);
    point.assign(V, 0);
    blo.assign(V / sqrtV + 1, 0);
  }

  void add(int x, T v) {
    assert(0 <= x && x < V);
    int bel = x / sqrtV;
    blo[bel] += v;
    point[x] += v;
  }

  T query(int x) const {
    assert(0 <= x && x <= V);
    T res = 0;
    int bel = x / sqrtV;
    for (int i = 0; i < bel; i++)
      res += blo[i];
    int start = bel * sqrtV;
    int end = x;
    for (int i = start; i < end; i++)
      res += point[i];
    return res;
  }

  T prod(int l, int r) const {
    assert(0 <= l && l <= r && r <= V);
    return query(r) - query(l);
  }
};

template <class T> struct fenwick : atcoder::fenwick_tree<T> {
  using atcoder::fenwick_tree<T>::fenwick_tree;
  using atcoder::fenwick_tree<T>::add;
  T query(int x) { return this->sum(x); }
  T prod(int l, int r) { return this->sum(l, r); }
};

} // namespace noya

#endif // NOYA_POINT_ADD_RANGE_SUM_HPP