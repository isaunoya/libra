#ifndef NOYA_RUNS_HPP
#define NOYA_RUNS_HPP 1

#include "noya/rollinghash.hpp"
#include <ranges>
#include <tuple>
#include <vector>

template<class T>
std::vector<std::tuple<int, int, int>> runs(const std::vector<T> &s) {
  int n = int(s.size());
  noya::double_hash_array<mint1, mint2, T> H;
  H.build(s);
  std::vector<std::tuple<int, int, int>> runs;
  for (bool inv : {false, true}) {
    std::vector<int> lyn(n, n), stack;
    for (int i = 0; i < n; i += 1) {
      while (!stack.empty()) {
        int j = stack.back(), k = lcp(H, i, H, j);
        if (i + k < n && ((s[i + k] > s[j + k]) ^ inv))
          break;
        lyn[j] = i;
        stack.pop_back();
      }
      stack.push_back(i);
    }
    for (int i = 0; i < n; i += 1) {
      int j = lyn[i], t = j - i, l = i - lcs(H, i - 1, H, j - 1),
          r = j + lcp(H, i, H, j);
      if (r - l >= 2 * t)
        runs.emplace_back(t, l, r);
    }
  }
  std::ranges::sort(runs);
  runs.erase(std::ranges::unique(runs).begin(), runs.end());
  return runs;
}
std::vector<std::tuple<int, int, int>> runs(const std::string &s) {
  std::vector<char> a(s.begin(), s.end());
  return runs(a);
}


#endif // NOYA_RUNS_HPP