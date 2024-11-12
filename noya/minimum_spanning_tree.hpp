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

#endif // NOYA_MINIMUM_SPANNING_TREE_HPP