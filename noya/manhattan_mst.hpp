#ifndef NOYA_MANHATTAN_MST
#define NOYA_MANHATTAN_MST 1

#include <algorithm>
#include <map>
#include <numeric>
#include <tuple>
#include <vector>

template <typename T>
std::vector<std::tuple<T, int, int>> manhattan_mst(std::vector<T> xs,
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

#endif // NOYA_MANHATTAN_MST