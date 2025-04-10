#ifndef NOYA_KNAPSACK_HPP
#define NOYA_KNAPSACK_HPP 1

#include "noya/maxplusconvolution.hpp"

#include <algorithm>
#include <limits>
#include <vector>

// item(weight, value), O(NlogN + L^2)
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

// https://qoj.ac/problem/7403
// O(n max{A})
template<class T>
T max_subsetsum_leq(const T &C, const std::vector<int> &A) {
  int N = int(A.size());
  int p = -1;
  T cur = 0;
  for (int i = 0; i < N; i++) {
    if (cur + A[i] > C) {
      p = i;
      break;
    } else {
      cur += A[i];
    }
  }
  if (p == -1) {
    return cur;
  }
  const int shift = *std::max_element(A.begin(), A.end());
  std::vector<int> dp0(2 * shift + 1);
  std::vector<int> dp1(2 * shift + 1);

  for (int i = 0; i <= shift; i++)
    dp0[i] = -1;

  dp0[cur - C + shift] = p;

  for (int i = p; i < N; i++) {
    dp1 = dp0;
    for (int j = 0; j <= shift; j++)
      dp1[j + A[i]] = std::max(dp1[j + A[i]], dp0[j]);

    for (int j = shift + A[i]; j >= shift + 1; j--)
      for (int k = dp1[j] - 1; k >= dp0[j]; k--)
        dp1[j - A[k]] = std::max(dp1[j - A[k]], k);
    dp0 = dp1;
  }

  for (int i = shift; i >= 0; i--)
    if (dp0[i] >= 0)
      return i - shift + C;
}

#include <bitset>

// O(N^2/w)
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