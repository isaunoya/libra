#ifndef NOYA_SUBSET_SUM_COUNT_HPP
#define NOYA_SUBSET_SUM_COUNT_HPP 1

#include "atcoder/convolution.hpp"
#include "noya/binom.hpp"

// @mod 998244353
std::vector<atcoder::modint998244353> solve(const std::vector<int> &A, int T) {
  using mint = atcoder::modint998244353;

  binom<mint> F(T + 1);
  std::vector<int> freq(T + 1, 0);
  int zero_count = 0;
  for (auto &a : A)
    if (a == 0)
      zero_count++;
    else
      freq[a]++;
}

#endif // NOYA_SUBSET_SUM_COUNT_HPP