#ifndef NOYA_SHORTEST_PATH_HPP
#define NOYA_SHORTEST_PATH_HPP 1

#include "noya/limit.hpp"

#include <algorithm>
#include <queue>
#include <vector>

// @(dis, pre)
std::pair<std::vector<int>, std::vector<int>>
bfs_unweighted(std::vector<std::vector<int>> &g, int start) {
  int N = int(g.size());
  const int INF = noya::infty<int>;
  std::vector<int> dis(N, INF);
  std::vector<int> pre(N, -1);
  dis[start] = 0;
  pre[start] = start;

  std::vector<int> que{start};
  for (int i = 0; i < int(que.size()); i++) {
    int u = que[i];
    for (auto v : g[u])
      if (dis[v] == INF) {
        dis[v] = dis[u] + 1;
        pre[v] = u;
        que.push_back(v);
      }
  }
  return {dis, pre};
}

// @(dis, pre)
template <class T>
std::pair<std::vector<int64_t>, std::vector<int>>
dijkstra(std::vector<std::vector<T>> &g, int start) {
  int N = int(g.size());
  const int64_t INF = noya::infty<int64_t>;
  std::vector<int64_t> dis(N, INF);
  std::vector<int> pre(N, -1);
  std::priority_queue<std::pair<int64_t, int>,
                      std::vector<std::pair<int64_t, int>>, std::greater<>>
      que;
  que.emplace(dis[start] = 0, start);
  pre[start] = start;
  while (!que.empty()) {
    auto [d, u] = que.top();
    que.pop();
    if (d > dis[u])
      continue;
    for (auto &[v, w] : g[u])
      if (dis[v] > dis[u] + w) {
        dis[v] = dis[u] + w;
        pre[v] = u;
        que.emplace(dis[v], v);
      }
  }
  return {dis, pre};
}

#include <cassert>

std::vector<int> find_path(std::vector<int> &pre, int s, int t) {
  std::vector<int> path;
  int cur = t;
  while (cur != s) {
    assert(cur >= 0);
    path.push_back(cur);
    cur = pre[cur];
  }
  path.push_back(s);
  std::reverse(path.begin(), path.end());
  return path;
}

template <class T>
std::vector<std::vector<int64_t>> floyd(std::vector<std::vector<T>> &g) {
  int N = int(g.size());
  const int64_t INF = noya::infty<int64_t>;
  std::vector<std::vector<int64_t>> f(N, std::vector<int64_t>(N, INF));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      f[i][j] = g[i][j];
  for (int i = 0; i < N; i++)
    f[i][i] = 0;
  for (int k = 0; k < N; k++)
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        f[i][j] = std::min(f[i][j], f[i][k] + f[k][j]);
  return f;
}

#endif // NOYA_SHORTEST_PATH_HPP