#ifndef NOYA_ROOTED_TREE_ISOMORPHISM_HPP
#define NOYA_ROOTED_TREE_ISOMORPHISM_HPP 1

#include <algorithm>
#include <map>
#include <vector>

std::vector<int> rooted_tree_isomorphism(const std::vector<std::vector<int>> &g,
                                         const int &root = 0) {
  int N = int(g.size());
  static std::map<std::vector<int>, int> mp;
  std::vector<int> ans(N);
  int cnt = 0;
  auto dfs = [&](auto &dfs, int u, int p) -> int {
    std::vector<int> sons;
    for (auto v : g[u]) {
      if (v == p)
        continue;
      sons.push_back(dfs(dfs, v, u));
    }
    std::sort(sons.begin(), sons.end());
    if (!mp.count(sons))
      mp[sons] = cnt++;
    return ans[u] = mp[sons];
  };

  dfs(dfs, root, -1);
  return ans;
}

#endif // NOYA_ROOTED_TREE_ISOMORPHISM_HPP