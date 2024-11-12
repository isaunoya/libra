#ifndef NOYA_POINT_ADD_REC_SUM_HPP
#define NOYA_POINT_ADD_REC_SUM_HPP 1

#include "noya/persistent_segtree.hpp"

namespace noya {
template <class T> struct dynamic_point_add_rectangle_sum {
  Persistent_segtree<T> seg;

  std::vector<std::array<T, 3>> weighted_points;
  std::vector<std::array<T, 3>> buckets;

  dynamic_point_add_rectangle_sum() {}
  dynamic_point_add_rectangle_sum(std::vector<std::array<T, 3>> &points) {
    buckets = points;
    build();
  };

  const int B = 6000;
  void add_points(T x, T y, T w) {
    buckets.push_back({x, y, w});
    if (int(buckets.size()) >= B) {
      build();
    }
  }

  void build() {
    if (!buckets.empty()) {
      for (auto &[x, y, w] : buckets) {
        weighted_points.push_back({x, y, w});
      }
      buckets.clear();
    }
    seg.build(weighted_points);
  }

  T query(T l, T r, T d, T u) {
    T ans = 0;
    ans = seg.prod(l, r, d, u);
    for (auto &[x, y, w] : buckets) {
      if (l <= x && x < r && d <= y && y < u) {
        ans += w;
      }
    }
    return ans;
  }
};
} // namespace noya

#endif // NOYA_POINT_ADD_REC_SUM_HPP