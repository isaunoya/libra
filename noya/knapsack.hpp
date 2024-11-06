#ifndef NOYA_KNAPSACK_HPP
#define NOYA_KNAPSACK_HPP 1

#include "noya/maxplusconvolution.hpp"

#include <algorithm>
#include <limits>
#include <vector>

// (weight, value)
template <class P>
std::vector<int64_t> knapsack(int L, const std::vector<P> &items) {
  std::vector<std::vector<int>> buckets(L + 1);
  for (auto &[w, v] : items) {
    assert(w >= 0);
    assert(v >= 0);
    if (w <= L) {
      buckets[w].push_back(v);
    }
  }

  std::vector<int64_t> dp(L + 1, std::numeric_limits<int64_t>::lowest());
  dp[0] = 0;

  for (int w = 1; w <= L; w++) {
    auto &bucket = buckets[w];
    if (bucket.empty()) {
      continue;
    }
    std::sort(bucket.begin(), bucket.end(), std::greater<>());
    const int m = std::min(int(bucket.size()), L / w);
    std::vector<int64_t> sum(m + 1);
    for (int i = 0; i < m; i++) {
      sum[i + 1] = sum[i] + bucket[i];
    }
    // @remainder
    for (int r = 0; r < w; r++) {
      const int n = int(L - r) / w + 1;
      std::vector<int64_t> v(n);
      for (int i = 0; i < n; i++) {
        v[i] = dp[i * w + r];
      }
      const std::vector<int64_t> &res = concave_maxplus_convolution(v, sum);
      for (int i = 0; i < n; i++) {
        dp[i * w + r] = res[i];
      }
    }
  }
  return dp;
}

#include <bitset>
template <int N> std::bitset<N> bool_knapsack(const std::vector<int> &items) {
  std::vector<int> freq(N);
  for (auto &item : items) {
    freq[item] += 1;
  }
  for (int i = 1; i < N; i++) {
    if (freq[i] >= 3) {
      int a = (freq[i] - 1) / 2;
      freq[i * 2] += a;
      freq[i] -= a * 2;
    }
  }
  std::bitset<N> res;
  res[0] = 1;
  for (int i = 1; i < N; i++) {
    for (int j = 0; j < freq[i]; j++) {
      res |= res << i;
    }
  }
  return res;
}

#endif // NOYA_KNAPSACK_HPP