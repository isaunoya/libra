#ifndef NOYA_CYCLE_HPP
#define NOYA_CYCLE_HPP 1

#include <numeric>
#include <vector>

bool cycle_detection_directed(const std::vector<std::vector<int>> &g) {
  int N = int(g.size());
  std::vector<int> deg(N);
  for (auto &gi : g) {
    for (auto &v : gi) {
      deg[v] += 1;
    }
  }
  std::vector<int> que;
  for (int i = 0; i < N; i++) {
    if (deg[i] == 0) {
      que.push_back(i);
    }
  }

  for (int i = 0; i < int(que.size()); i++) {
    int u = que[i];
    for (auto v : g[u]) {
      deg[v] -= 1;
      if (!deg[v]) {
        que.push_back(v);
      }
    }
  }
  return std::accumulate(deg.begin(), deg.end(), 0) > 0;
}

#include "atcoder/dsu.hpp"
#include <algorithm>

bool cycle_detection_undirected(const std::vector<std::pair<int, int>> &edge) {
  int N = 0;
  for (auto &[a, b] : edge) {
    N = std::max(N, a);
    N = std::max(N, b);
  }
  N++;
  atcoder::dsu f(N);
  for (auto &[a, b] : edge) {
    if (f.same(a, b)) {
      return true;
    }
    f.merge(a, b);
  }
  return false;
}

#endif // NOYA_CYCLE_HPP