#ifndef NOYA_FASTLCA_HPP
#define NOYA_FASTLCA_HPP 1

#include <algorithm>
#include <functional>
#include <noya/sparsetable>
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
  std::vector<int> siz;
  sparse_table<std::pair<int, int>, min_op> ST;
  fastlca(const std::vector<std::vector<int>> &g = {}, const int &root = 0) {
    if (!g.empty())
      build(g, root);
  };

  void build(const std::vector<std::vector<int>> &g = {}, const int &root = 0) {
    n = int(g.size());
    d.assign(n, 0);
    dfn.assign(n, -1);
    siz.assign(n, 0);

    int idx = 0;
    std::vector<std::pair<int, int>> a;
    a.reserve(n);

    auto dfs = [&](auto &dfs, int u, int p) -> void {
      siz[u] = 1;
      dfn[u] = idx++;
      if (p == -1)
        a.push_back({-1, -1});
      else
        a.push_back(std::make_pair(dfn[p], p));

      for (auto v : g[u]) {
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

  int distance(int a, int b) const { return d[a] + d[b] - d[lca(a, b)] * 2; }

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

#endif // NOYA_FASTLCA_HPP