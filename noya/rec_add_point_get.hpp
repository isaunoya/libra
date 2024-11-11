#ifndef NOYA_REC_ADD_POINT_GET_HPP
#define NOYA_REC_ADD_POINT_GET_HPP 1

#include "noya/rectangle_sum.hpp"

// @areas(l, r, d, u)
template <class T, class C = noya::fenwick<T>>
std::vector<T> static_rectangle_add_point_get(std::vector<std::array<int, 5>> &areas,
                             std::vector<std::array<int, 2>> &points) {
  std::vector<std::array<int, 3>> weighted_points;
  for (auto &[l, r, d, u, w] : areas) {
    weighted_points.push_back({l, d, w});
    weighted_points.push_back({l, u, -w});

    weighted_points.push_back({r, d, -w});
    weighted_points.push_back({r, u, w});
  }
  std::vector<std::array<int, 4>> queries;
  for (auto &[x, y] : points) {
    queries.push_back({0, x + 1, 0, y + 1});
  }
  return rectangle_sum<T, C>(weighted_points, queries);
}

#endif // NOYA_REC_ADD_POINT_GET_HPP