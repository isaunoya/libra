#ifndef NOYA_PERSISTENT_SEGTREE_HPP
#define NOYA_PERSISTENT_SEGTREE_HPP 1

#include "noya/limit.hpp"

#include <algorithm>
#include <vector>

namespace noya {
template <class T> struct Persistent_segtree {
  std::vector<int> rt;
  std::vector<int> ls, rs;
  int N, M;
  std::vector<T> Xs, Ys;

  std::vector<T> sum;
  int cnt, C;

  Persistent_segtree() {}

  Persistent_segtree(std::vector<std::array<T, 3>> &points) {
    if (!points.empty())
      build(points);
  }

  void update(int pre, int &p, int l, int r, int x, T v) {
    p = cnt++;
    ls[p] = ls[pre];
    rs[p] = rs[pre];
    sum[p] = sum[pre] + v;
    if (l + 1 == r)
      return;
    int m = (l + r) / 2;
    if (x < m)
      update(ls[pre], ls[p], l, m, x, v);
    else
      update(rs[pre], rs[p], m, r, x, v);
  }

  int query_kth(int L, int R, int l, int r, T k) {
    if (l + 1 == r) {
      return l;
    }
    int m = (l + r) / 2;
    T val = sum[ls[R]] - sum[ls[L]];
    if (val >= k) {
      return query_kth(ls[L], ls[R], l, m, k);
    } else {
      return query_kth(rs[L], rs[R], m, r, k - val);
    }
  }

  T query_prefix(int p, int l, int r, int x) {
    if (!p)
      return 0;

    if (l + 1 == r)
      return sum[p];

    int m = (l + r) / 2;
    if (x < m)
      return query_prefix(ls[p], l, m, x);
    else
      return sum[ls[p]] + query_prefix(rs[p], m, r, x);
  }

  T query_single(int p, int l, int r, int x) {
    if (!p)
      return 0;

    if (l + 1 == r)
      return sum[p];

    int m = (l + r) / 2;
    if (x < m)
      return query_single(ls[p], l, m, x);
    else
      return query_single(rs[p], m, r, x);
  }

  T query(int L, int R, int ql, int qr, int l, int r) {
    if (ql <= l && r <= qr)
      return sum[R] - sum[L];

    int m = (l + r) / 2;
    T res = 0;
    if (ql < m)
      res += query(ls[L], ls[R], ql, qr, l, m);
    if (qr > m)
      res += query(rs[L], rs[R], ql, qr, m, r);

    return res;
  }

  T prod(T r, T u) {
    r = std::lower_bound(Xs.begin(), Xs.end(), r) - Xs.begin();
    u = std::lower_bound(Ys.begin(), Ys.end(), u) - Ys.begin();

    assert(0 <= r && r <= N);
    assert(0 <= u && u <= M);

    if (u > 0)
      return query_prefix(rt[r], 0, M, u - 1);
    else
      return 0;
  }

  T prod(T l, T r, T d, T u) {
    l = std::lower_bound(Xs.begin(), Xs.end(), l) - Xs.begin();
    r = std::lower_bound(Xs.begin(), Xs.end(), r) - Xs.begin();

    d = std::lower_bound(Ys.begin(), Ys.end(), d) - Ys.begin();
    u = std::lower_bound(Ys.begin(), Ys.end(), u) - Ys.begin();

    assert(0 <= l && l <= r && r <= N);
    assert(0 <= d && d <= u && u <= M);

    if (l == r)
      return 0;
    else if (d == u)
      return 0;
    else
      return query(rt[l], rt[r], d, u, 0, M);
  }

  T kth(T l, T r, T k) {
    l = std::lower_bound(Xs.begin(), Xs.end(), l) - Xs.begin();
    r = std::lower_bound(Xs.begin(), Xs.end(), r) - Xs.begin();
    assert(0 <= l && l <= r && r <= N);

    int res = query_kth(rt[l], rt[r], 0, M, k);
    if (sum[rt[r]] - sum[rt[l]] < k)
      return noya::infty<T>;
    else
      return Ys[res];
  }

  void build(std::vector<std::array<T, 3>> points) {
    C = int(points.size()) * 30;
    cnt = 1;

    ls.assign(C, 0);
    rs.assign(C, 0);
    sum.assign(C, 0);

    Xs.clear();
    Ys.clear();

    for (auto &[x, y, w] : points) {
      Xs.push_back(x);
      Ys.push_back(y);
    }

    std::sort(Xs.begin(), Xs.end());
    std::sort(Ys.begin(), Ys.end());
    Xs.erase(std::unique(Xs.begin(), Xs.end()), Xs.end());
    Ys.erase(std::unique(Ys.begin(), Ys.end()), Ys.end());
    N = int(Xs.size());

    std::vector<std::vector<std::array<T, 2>>> add(N);
    for (auto &[x, y, w] : points) {
      x = std::lower_bound(Xs.begin(), Xs.end(), x) - Xs.begin();
      y = std::lower_bound(Ys.begin(), Ys.end(), y) - Ys.begin();
      add[x].push_back({y, w});
    }

    M = int(Ys.size());
    rt.assign(N + 1, 0);
    
    rt[0] = cnt++;
    for (int i = 0; i < N; i++) {
      rt[i + 1] = rt[i];
      for (auto &[y, w] : add[i]) {
        int new_rt = 0;
        update(rt[i + 1], new_rt, 0, M, y, w);
        rt[i + 1] = new_rt;
      }
    }

    std::vector<std::vector<std::array<T, 2>>>().swap(add);
  }
  // @(ls, rs, len, value)
  std::vector<std::array<int, 4>> all_nodes() {
    std::vector<std::array<int, 4>> nodes(cnt, std::array<int, 4>{});
    std::vector<bool> vis(cnt);
    vis[0] = true;
    for (int i = 0; i <= N; i++) {
      auto dfs = [&](auto &dfs, int p, int l, int r) -> void {
        if (vis[p])
          return;
        vis[p] = true;
        nodes[p] = {ls[p], rs[p], r - l, sum[p]};
        int m = (l + r) / 2;
        dfs(dfs, ls[p], l, m);
        dfs(dfs, rs[p], m, r);
      };
      dfs(dfs, rt[i], 0, M);
    }
    return nodes;
  }
};
} // namespace noya
#endif // NOYA_PERSISTENT_SEGTREE_HPP