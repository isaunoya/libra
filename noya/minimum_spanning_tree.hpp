#ifndef NOYA_MINIMUM_SPANNING_TREE_HPP
#define NOYA_MINIMUM_SPANNING_TREE_HPP 1

#include "atcoder/dsu.hpp"
#include <numeric>
// @(w, u, v)
template <class T>
std::pair<int64_t, std::vector<int>>
minimum_spanning_tree(int N, const std::vector<T> &edges) {
  int64_t ans = 0;
  std::vector<int> idx;
  atcoder::dsu f(N);
  int m = int(edges.size());
  std::vector<int> p(m);
  std::iota(p.begin(), p.end(), 0);
  std::sort(p.begin(), p.end(),
            [&](int i, int j) { return edges[i] < edges[j]; });

  for (auto &i : p) {
    auto &[w, u, v] = edges[i];
    assert(0 <= u && u < N);
    assert(0 <= v && v < N);
    if (!f.same(u, v)) {
      ans += w;
      f.merge(u, v);
      idx.push_back(i);
    }
  }
  return make_pair(ans, idx);
}

// @(w, u, v)
template <class T>
std::pair<int64_t, std::vector<int>>
maximum_spanning_tree(int N, const std::vector<T> &edges) {
  int64_t ans = 0;
  std::vector<int> idx;
  atcoder::dsu f(N);
  int m = int(edges.size());
  std::vector<int> p(m);
  std::iota(p.begin(), p.end(), 0);
  std::sort(p.begin(), p.end(),
            [&](int i, int j) { return edges[i] > edges[j]; });

  for (auto &i : p) {
    auto &[w, u, v] = edges[i];
    assert(0 <= u && u < N);
    assert(0 <= v && v < N);
    if (!f.same(u, v)) {
      ans += w;
      f.merge(u, v);
      idx.push_back(i);
    }
  }
  return make_pair(ans, idx);
}

#include "noya/limit.hpp"
// @(w, u, v)
template <class T>
std::pair<int64_t, std::vector<int>> prim_dense(int N,
                                                const std::vector<T> &edges) {
  std::vector<std::vector<int>> id(N, std::vector<int>(N, -1));
  const int M = int(edges.size());
  for (int i = 0; i < M; i++) {
    auto [w, u, v] = edges[i];
    assert(0 <= u && u < N);
    assert(0 <= v && v < N);
    id[u][v] = id[v][u] = i;
  }
  std::vector<int> idx;
  std::vector<int> vis(N, 0);
  std::vector<int> dis(N, -1);
  auto cmp = [&](int a, int b) -> int {
    if (a == -1)
      return b;
    if (b == -1)
      return a;
    auto [w1, u1, v1] = edges[a];
    auto [w2, u2, v2] = edges[b];
    return w1 < w2 ? a : b;
  };

  int64_t ans = 0;
  int k = 0;
  for (int t = 1; t < N; t++) {
    vis[k] = 1;
    int nx = -1;
    for (int i = 0; i < N; i++) {
      if (!vis[i]) {
        dis[i] = cmp(dis[i], id[k][i]);
        nx = cmp(nx, dis[i]);
      }
    }
    if (nx == -1)
      break;
    idx.push_back(nx);
    {
      auto [w, u, v] = edges[nx];
      ans += w;
      if (vis[u]) {
        k = v;
      } else {
        k = u;
      }
    }
  }
  return make_pair(ans, idx);
}

#endif // NOYA_MINIMUM_SPANNING_TREE_HPP