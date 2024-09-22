#ifndef NOYA_BM_HPP
#define NOYA_BM_HPP 1

#include <bitset>
#include <cassert>
#include <vector>

namespace noya {
template <typename BS> struct BipartiteMatching_Dense {
  int N1, N2;
  std::vector<BS> &adj;
  std::vector<int> match_1, match_2;
  std::vector<int> que;
  std::vector<int> prev;
  BS vis;

  BipartiteMatching_Dense(std::vector<BS> &adj, int N1, int N2)
      : N1(N1), N2(N2), adj(adj), match_1(N1, -1), match_2(N2, -1) {
    for (int s = 0; s < N1; s++)
      bfs(s);
  }

  void bfs(int s) {
    if (match_1[s] != -1)
      return;
    que.resize(N1), prev.resize(N1);
    int l = 0, r = 0;
    vis.set(), prev[s] = -1;

    que[r++] = s;
    while (l < r) {
      int u = que[l++];
      BS cand = vis & adj[u];
      for (int v = cand._Find_first(); v < N2; v = cand._Find_next(v)) {
        vis[v] = 0;
        if (match_2[v] != -1) {
          que[r++] = match_2[v];
          prev[match_2[v]] = u;
          continue;
        }
        int a = u, b = v;
        while (a != -1) {
          int t = match_1[a];
          match_1[a] = b, match_2[b] = a, a = prev[a], b = t;
        }
        return;
      }
    }
    return;
  }

  std::vector<std::pair<int, int>> matching() {
    std::vector<std::pair<int, int>> res;
    for (int v = 0; v < N1; v++)
      if (match_1[v] != -1)
        res.emplace_back(v, match_1[v]);
    return res;
  }

  std::pair<std::vector<int>, std::vector<int>> vertex_cover() {
    std::vector<int> que(N1);
    int l = 0, r = 0;
    vis.set();
    std::vector<bool> done(N1);
    for (int i = 0; i < N1; i++) {
      if (match_1[i] == -1)
        done[i] = 1, que[r++] = i;
    }
    while (l < r) {
      int a = que[l++];
      BS cand = adj[a] & vis;
      for (int b = cand._Find_first(); b < N2; b = cand._Find_next(b)) {
        vis[b] = 0;
        int to = match_2[b];
        assert(to != -1);
        if (!done[to])
          done[to] = 1, que[r++] = to;
      }
    }
    std::vector<int> left, right;
    for (int i = 0; i < N1; i++)
      if (!done[i])
        left.emplace_back(i);
    for (int i = 0; i < N2; i++)
      if (!vis[i])
        right.emplace_back(i);
    return {left, right};
  }
};
} // namespace noya

#endif // NOYA_BM_HPP
