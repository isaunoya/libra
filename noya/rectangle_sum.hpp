#ifndef NOYA_RECTANGLE_SUM_HPP
#define NOYA_RECTANGLE_SUM_HPP 1

#include "noya/point_add_range_sum.hpp"

#include <algorithm>
#include <array>

// @points(x, y, weight) @queries(l, r, d, u)
template <class T, class C = noya::fenwick<T>>
std::vector<T> rectangle_sum(std::vector<std::array<int, 3>> points,
                             std::vector<std::array<int, 4>> queries) {
  std::vector<int> Xs;
  std::vector<int> Ys;
  for (auto &[x, y, z] : points) {
    Xs.push_back(x);
    Ys.push_back(y);
  }
  for (auto &[l, r, d, u] : queries) {
    Xs.push_back(l);
    Xs.push_back(r);
    Ys.push_back(d);
    Ys.push_back(u);
  }

  std::sort(Xs.begin(), Xs.end());
  Xs.erase(std::unique(Xs.begin(), Xs.end()), Xs.end());

  std::sort(Ys.begin(), Ys.end());
  Ys.erase(std::unique(Ys.begin(), Ys.end()), Ys.end());

  for (auto &[x, y, z] : points) {
    x = std::lower_bound(Xs.begin(), Xs.end(), x) - Xs.begin();
    y = std::lower_bound(Ys.begin(), Ys.end(), y) - Ys.begin();
  }

  for (auto &[l, r, d, u] : queries) {
    l = std::lower_bound(Xs.begin(), Xs.end(), l) - Xs.begin();
    r = std::lower_bound(Xs.begin(), Xs.end(), r) - Xs.begin();
    d = std::lower_bound(Ys.begin(), Ys.end(), d) - Ys.begin();
    u = std::lower_bound(Ys.begin(), Ys.end(), u) - Ys.begin();
  }

  int X = int(Xs.size());
  int Y = int(Ys.size());
  C F(Y);
  std::vector<std::vector<std::array<int, 3>>> Q(X + 1);
  for (int i = 0; i < int(queries.size()); i++) {
    auto &[l, r, d, u] = queries[i];
    Q[r].push_back({i, d, u});
    Q[l].push_back({~i, d, u});
  }

  std::vector<std::vector<std::array<int, 2>>> A(X + 1);
  for (auto &[x, y, z] : points) {
    A[x].push_back({y, z});
  }

  std::vector<T> ans(queries.size());
  for (int x = 0; x < X; x++) {
    for (auto &[y, z] : A[x]) {
      F.add(y, z);
    }
    for (auto &[i, d, u] : Q[x + 1]) {
      if (i >= 0)
        ans[i] += F.prod(d, u);
      else
        ans[~i] -= F.prod(d, u);
    }
  }
  return ans;
}

#endif // NOYA_RECTANGLE_SUM_HPP