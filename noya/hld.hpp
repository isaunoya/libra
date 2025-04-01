#ifndef NOYA_HLD_HPP
#define NOYA_HLD_HPP 1

#include <algorithm>
#include <cassert>
#include <vector>
#include <array>

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

  std::vector<int> tour_list;
  void dfs2(int u, int t) {
    top[u] = t, dfn[u] = idx++;
    tour_list.push_back(u);
    if (son[u] != -1)
      dfs2(son[u], t);
    for (auto v : G[u])
      if (top[v] == -1)
        dfs2(v, v);
  }

  int get_kth_ancestor(int a, int k) const {
    if (k > d[a])
      return -1;

    int goal = d[a] - k;
    while (d[top[a]] > goal)
      a = fa[top[a]];

    int pos = dfn[a] - (d[a] - goal);
    return tour_list[pos];
  }

  int get_kth_node_on_path(int a, int b, int k) const {
    int anc = lca(a, b);
    int first_half = d[a] - d[anc];
    int second_half = d[b] - d[anc];

    if (k < 0 || k > first_half + second_half)
      return -1;

    if (k < first_half)
      return get_kth_ancestor(a, k);
    else
      return get_kth_ancestor(b, first_half + second_half - k);
  }

  HLD(const std::vector<std::vector<int>> &g = {}, const int &root = 0) {
    if (!g.empty())
      build(g, root);
  }

  void build(const std::vector<std::vector<int>> &g = {}, const int &root = 0) {
    n = g.size();
    G = g;
    siz.assign(n, 0);
    dfn.assign(n, -1);
    son.assign(n, -1);
    top.assign(n, -1);
    d.assign(n, 0);
    fa.assign(n, 0);
    d[root] = 1;
    dfs(root, -1);
    idx = 0;
    dfs2(root, root);
  }

  // a in subtree(b)
  bool is_subtree(int a, int b) const {
    if (dfn[b] <= dfn[a] && dfn[a] < dfn[b] + siz[b]) {
      return true;
    } else {
      return false;
    }
  }

  int lca(int x, int y) const {
    while (top[x] != top[y]) {
      if (d[top[x]] < d[top[y]])
        std::swap(x, y);
      x = fa[top[x]];
    }
    return d[x] < d[y] ? x : y;
  }

  // @(l, r, dir), dir=0 indicates x->lca (need reverse info)
  std::vector<std::tuple<int, int, bool>> chain(int x, int y) const {
    assert(0 <= x && x < n);
    assert(0 <= y && y < n);
    std::vector<std::tuple<int, int, bool>> L, R;
    while (top[x] != top[y]) {
      assert(0 <= x && x < n);
      assert(0 <= y && y < n);
      if (d[top[x]] > d[top[y]]) {
        L.emplace_back(dfn[top[x]], dfn[x] + 1, false);
        x = fa[top[x]];
      } else {
        R.emplace_back(dfn[top[y]], dfn[y] + 1, true);
        y = fa[top[y]];
      }
    }
    if (dfn[y] < dfn[x])
      L.emplace_back(dfn[y], dfn[x] + 1, false);
    else
      R.emplace_back(dfn[x], dfn[y] + 1, true);
    reverse(R.begin(), R.end());
    L.insert(L.end(), R.begin(), R.end());
    return L;
  }

  std::array<int, 2> subtree(int a) const { return {dfn[a], dfn[a] + siz[a]}; }

  int rooted_lca(int a, int b, int c) const {
    return lca(a, b) ^ lca(a, c) ^ lca(b, c);
  }

  std::pair<int, int> intersection(int a, int b, int c, int d) const {
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
                                   std::pair<int, int> b) const {
    return intersection(a.first, a.second, b.first, b.second);
  }
};
} // namespace noya

#endif // NOYA_HLD_HPP