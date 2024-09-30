#ifndef NOYA_RANDOM_HPP
#define NOYA_RANDOM_HPP 1
#include <algorithm>
#include <ctime>
#include <numeric>
#include <random>
#include <vector>

namespace noya {
using ull = unsigned long long;
namespace internal {
std::mt19937_64 gen_values(time(0));
} // namespace internal

std::vector<int> random_permutation(int N) {
  std::vector<int> p(N);
  std::iota(p.begin(), p.end(), 0);
  std::shuffle(p.begin(), p.end(), internal::gen_values);
  return p;
}
std::vector<ull> random_hash_values(int N) {
  std::vector<ull> X(N);
  std::generate(X.begin(), X.end(), internal::gen_values);
  return X;
}
std::vector<std::vector<int>> random_tree(int N) {
  std::vector<std::vector<int>> g(N);
  for (int i = 1; i < N; i++) {
    int p = internal::gen_values() % i;
    g[p].push_back(i);
  }
  return g;
}
} // namespace noya

#endif // NOYA_RANDOM_HPP