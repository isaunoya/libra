#ifndef NOYA_FASTLCA_HPP
#define NOYA_FASTLCA_HPP 1

#include "noya/sparsetable.hpp"
#include "noya/limit.hpp"

#include <algorithm>
#include <functional>
#include <numeric>

namespace noya {
// space: O(n log n)
struct fastlca {
  static std::pair<int, int> min_op(std::pair<int, int> a,
                                    std::pair<int, int> b) {
    return std::min(a, b);
  }
  int n;
  std::vector<int> dfn;
  std::vector<int> d;
  std::vector<int64_t> len;
  std::vector<int> siz;
  sparse_table<std::pair<int, int>, min_op> ST;
  bool weighted;

  std::vector<int> fa;

  template <class T>
  fastlca(const std::vector<T> &g = {}, const bool &_weighted = false,
          const int &root = 0) {
    weighted = _weighted;
    if (!g.empty())
      build(g, root);
  };

  template <class T>
  void build(const std::vector<std::vector<T>> &g, const int &root = 0) {
    n = int(g.size());
    std::vector<std::vector<int>> g2(n);

    for (int u = 0; u < n; u++) {
      for (auto &[v, w] : g[u]) {
        g2[u].push_back(v);
      }
    }
    build(g2, root);

    len.assign(n, 0);
    auto dfs = [&](auto &dfs, int u) -> void {
      for (auto &[v, w] : g[u]) {
        if (v == fa[u])
          continue;
        len[v] = len[u] + w;
        dfs(dfs, v);
      }
    };

    dfs(dfs, root);
  }

  void build(const std::vector<std::vector<int>> &g = {}, const int &root = 0) {
    n = int(g.size());
    d.assign(n, 0);
    dfn.assign(n, -1);
    siz.assign(n, 0);
    fa.assign(n, -1);

    int idx = 0;
    std::vector<std::pair<int, int>> a;
    a.reserve(n);

    auto dfs = [&](auto &dfs, int u, int p) -> void {
      fa[u] = p;
      siz[u] = 1;
      dfn[u] = idx++;
      if (p == -1)
        a.push_back({-1, -1});
      else
        a.push_back(std::make_pair(dfn[p], p));

      for (auto &v : g[u]) {
        if (v != p) {
          d[v] = d[u] + 1;
          dfs(dfs, v, u);
          siz[u] += siz[v];
        }
      }
    };

    dfs(dfs, root, -1);
    ST.build(a);
  }

  // a in subtree(b)
  bool is_subtree(int a, int b) {
    if (dfn[b] <= dfn[a] && dfn[a] < dfn[b] + siz[b]) {
      return true;
    } else {
      return false;
    }
  }

  int lca(int u, int v) const {
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);

    if (u == v) {
      return u;
    } else {
      int a = dfn[u];
      int b = dfn[v];
      if (a > b) {
        std::swap(a, b);
      }
      return ST.prod(a + 1, b + 1).second;
    }
  }

  int rooted_lca(int a, int b, int c) const {
    return lca(a, b) ^ lca(a, c) ^ lca(b, c);
  }

  int64_t distance(int a, int b) const {
    int c = lca(a, b);
    if (!weighted) {
      return d[a] + d[b] - d[c] * 2;
    } else {
      return len[a] + len[b] - len[c] * 2;
    }
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

  std::vector<int> path(int a, int b) {
    int c = lca(a, b);
    std::vector<int> ac;
    while (a != c) {
      ac.push_back(a);
      a = fa[a];
    }
    std::vector<int> bc;
    while (b != c) {
      bc.push_back(b);
      b = fa[b];
    }
    std::vector<int> res = move(ac);
    res.push_back(c);
    res.insert(res.end(), bc.rbegin(), bc.rend());
    return res;
  }
};
} // namespace noya

#endif // NOYA_FASTLCA_HPP