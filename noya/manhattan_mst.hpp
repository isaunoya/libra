#ifndef NOYA_MANHATTAN_MST_HPP
#define NOYA_MANHATTAN_MST_HPP 1

#include "noya/minimum_spanning_tree.hpp"

#include <algorithm>
#include <map>
#include <numeric>
#include <tuple>
#include <vector>

template <typename T>
std::vector<std::tuple<T, int, int>> manhattan_edges(std::vector<T> xs,
                                                     std::vector<T> ys) {
  const int n = xs.size();
  std::vector<int> idx(n);
  std::iota(idx.begin(), idx.end(), 0);
  std::vector<std::tuple<T, int, int>> ret;
  for (int s = 0; s < 2; s++) {
    for (int t = 0; t < 2; t++) {
      auto cmp = [&](int i, int j) { return xs[i] + ys[i] < xs[j] + ys[j]; };
      std::sort(idx.begin(), idx.end(), cmp);
      std::map<T, int> sweep;
      for (int i : idx) {
        for (auto it = sweep.lower_bound(-ys[i]); it != sweep.end();
             it = sweep.erase(it)) {
          int j = it->second;
          if (xs[i] - xs[j] < ys[i] - ys[j])
            break;
          ret.emplace_back(std::abs(xs[i] - xs[j]) + std::abs(ys[i] - ys[j]), i,
                           j);
        }
        sweep[-ys[i]] = i;
      }
      std::swap(xs, ys);
    }
    for (auto &x : xs)
      x = -x;
  }
  std::sort(ret.begin(), ret.end());
  return ret;
}

template <typename PointType>
auto manhattan_edges(const std::vector<PointType> &points) {
  assert(!points.empty());
  using CoordType = std::decay_t<decltype(std::get<0>(points[0]))>;
  std::vector<CoordType> xs, ys;
  for (const auto &point : points) {
    xs.push_back(std::get<0>(point));
    ys.push_back(std::get<1>(point));
  }
  return manhattan_edges(xs, ys);
}

#endif // NOYA_MANHATTAN_MST_HPP