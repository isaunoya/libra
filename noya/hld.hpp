#ifndef NOYA_HLD_HPP
#define NOYA_HLD_HPP 1

#include <algorithm>
#include <cassert>
#include <vector>

namespace noya {
struct HLD {
  HLD() {}
  std::vector<std::vector<int>> G;
  int n;
  std::vector<int> dfn, siz, son, top, d, fa;
  int idx;
  void dfs(int u, int p) {
    siz[u] = 1;
    for (auto v : G[u])
      if (v != p) {
        fa[v] = u;
        d[v] = d[u] + 1;
        dfs(v, u), siz[u] += siz[v];
        if (son[u] == -1 || siz[v] > siz[son[u]])
          son[u] = v;
      }
  }
  void dfs2(int u, int t) {
    top[u] = t, dfn[u] = idx++;
    if (son[u] != -1)
      dfs2(son[u], t);
    for (auto v : G[u])
      if (top[v] == -1)
        dfs2(v, v);
  }
  HLD(const std::vector<std::vector<int>> &g = {}, const int &root = 0) {
    if (!g.empty())
      build(g, root);
  }

  void build(const std::vector<std::vector<int>> &g = {}, const int &root = 0) {
    n = g.size();
    G = g;
    siz.resize(n);
    dfn.resize(n);
    son.resize(n, -1);
    top.resize(n, -1);
    d.resize(n);
    fa.resize(n);
    dfs(root, -1), idx = 0, dfs2(root, root);
  }

  // a in subtree(b)
  bool is_subtree(int a, int b) {
    if (dfn[b] <= dfn[a] && dfn[a] < dfn[b] + siz[b]) {
      return true;
    } else {
      return false;
    }
  }

  int lca(int x, int y) {
    while (top[x] != top[y]) {
      if (d[top[x]] < d[top[y]])
        std::swap(x, y);
      x = fa[top[x]];
    }
    return d[x] < d[y] ? x : y;
  }
  // L>=R, u->lca and L<=R, lca->v
  std::vector<std::pair<int, int>> chain(int x, int y) {
    assert(0 <= x && x < n);
    assert(0 <= y && y < n);
    std::vector<std::pair<int, int>> L, R;
    while (top[x] != top[y]) {
      assert(0 <= x && x < n);
      assert(0 <= y && y < n);
      if (d[top[x]] > d[top[y]]) {
        L.emplace_back(dfn[x], dfn[top[x]]);
        x = fa[top[x]];
      } else {
        R.emplace_back(dfn[top[y]], dfn[y]);
        y = fa[top[y]];
      }
    }
    L.emplace_back(dfn[x], dfn[y]);
    reverse(R.begin(), R.end());
    L.insert(L.end(), R.begin(), R.end());
    return L;
  }

  int rooted_lca(int a, int b, int c) {
    return lca(a, b) ^ lca(a, c) ^ lca(b, c);
  }

  std::pair<int, int> intersection(int a, int b, int c, int d) {
    int ab = lca(a, b), ac = lca(a, c), ad = lca(a, d);
    int bc = lca(b, c), bd = lca(b, d), cd = lca(c, d);
    int x = ab ^ ac ^ bc;
    int y = ab ^ ad ^ bd;
    if (x != y) {
      return {x, y};
    }
    int z = ac ^ ad ^ cd;
    if (x != z) {
      x = -1;
    }
    return {x, x};
  }

  std::pair<int, int> intersection(std::pair<int, int> a,
                                   std::pair<int, int> b) {
    return intersection(a.first, a.second, b.first, b.second);
  }
};
} // namespace noya

#endif // NOYA_HLD_HPP