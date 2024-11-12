#ifndef NOYA_REC_ADD_POINT_GET_HPP
#define NOYA_REC_ADD_POINT_GET_HPP 1

#include "noya/rectangle_sum.hpp"

// @areas(l, r, d, u)
template <class T, class C = noya::fenwick<T>>
std::vector<T>
static_rectangle_add_point_get(std::vector<std::array<int, 5>> areas,
                               std::vector<std::array<int, 2>> points) {
  std::vector<std::array<T, 3>> weighted_points;
  for (auto &[l, r, d, u, w] : areas) {
    weighted_points.push_back({l, d, w});
    weighted_points.push_back({l, u, -w});

    weighted_points.push_back({r, d, -w});
    weighted_points.push_back({r, u, w});
  }
  std::vector<std::array<T, 4>> queries;
  for (auto &[x, y] : points) {
    queries.push_back({0, x + 1, 0, y + 1});
  }
  return rectangle_sum<T, C>(weighted_points, queries);
}

#include "noya/persistent_segtree.hpp"
namespace noya {
template <class T> struct dynamic_rectangle_add_point_get {
  Persistent_segtree<T> seg;

  std::vector<std::array<T, 3>> weighted_points;
  std::vector<std::array<T, 5>> buckets;

  dynamic_rectangle_add_point_get() {}

  dynamic_rectangle_add_point_get(std::vector<std::array<T, 5>> &areas) {
    buckets = areas;
    build();
  };

  const int B = 6000;
  void add_rectangle(T l, T r, T d, T u, T w) {
    buckets.push_back({l, r, d, u, w});
    if (int(buckets.size()) >= B) {
      build();
    }
  }

  void build() {
    if (!buckets.empty()) {
      for (auto &[l, r, d, u, w] : buckets) {
        weighted_points.push_back({l, d, w});
        weighted_points.push_back({l, u, -w});
        weighted_points.push_back({r, d, -w});
        weighted_points.push_back({r, u, w});
      }
      buckets.clear();
    }
    seg.build(weighted_points);
  }

  T query(int x, int y) {
    T ans = 0;
    ans = seg.prod(x + 1, y + 1);
    for (auto &[l, r, d, u, w] : buckets) {
      if (l <= x && x < r && d <= y && y < u) {
        ans += w;
      }
    }
    return ans;
  }
};
} // namespace noya

#endif // NOYA_REC_ADD_POINT_GET_HPP