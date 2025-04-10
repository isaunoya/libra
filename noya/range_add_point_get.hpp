#ifndef NOYA_RANGE_ADD_POINT_GET_HPP
#define NOYA_RANGE_ADD_POINT_GET_HPP 1

#include "noya/point_add_range_sum.hpp"

template <class T, class C = noya::fenwick<T>> struct range_add_point_get {
  int N;
  C ST;
  range_add_point_get(int _N) : ST(_N), N(_N) {}

  void range_add(int l, int r, T v) {
    ST.add(l, v);
    if (r + 1 < N) {
      ST.add(r + 1, -v);
    }
  }

  T point_get(int x) { return ST.prod(0, x + 1); }
};

#endif // NOYA_RANGE_ADD_POINT_GET_HPP