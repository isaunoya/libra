#ifndef NOYA_BM_HPP
#define NOYA_BM_HPP 1

#include <algorithm>
#include <bitset>
#include <cassert>
#include <random>
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

// https://judge.yosupo.jp/submission/233075
struct HopcroftKarp {
  std::vector<int> g, l, r;
  int ans;
  HopcroftKarp(int n, int m, std::vector<std::pair<int, int>> &e)
      : g(e.size()), l(n, -1), r(m, -1), ans(0) {
    std::mt19937 rng(0);
    std::shuffle(e.begin(), e.end(), rng);
    std::vector<int> deg(n + 1), a, p, q(n);
    for (auto &[x, y] : e)
      deg[x]++;
    for (int i = 1; i <= n; i++)
      deg[i] += deg[i - 1];
    for (auto &[x, y] : e)
      g[--deg[x]] = y;
    for (bool match = true; match;) {
      a.assign(n, -1);
      p.assign(n, -1);
      int t = 0;
      match = false;
      for (int i = 0; i < n; i++)
        if (l[i] == -1)
          q[t++] = a[i] = p[i] = i;
      for (int i = 0; i < t; i++) {
        int x = q[i];
        if (~l[a[x]])
          continue;
        for (int j = deg[x]; j < deg[x + 1]; j++) {
          int y = g[j];
          if (r[y] == -1) {
            while (~y) {
              r[y] = x;
              std::swap(l[x], y);
              x = p[x];
            }
            match = true;
            ans++;
            break;
          }
          if (p[r[y]] == -1) {
            q[t++] = y = r[y];
            p[y] = x;
            a[y] = a[x];
          }
        }
      }
    }
  }
};

} // namespace noya

#endif // NOYA_BM_HPP
