#ifndef NOYA_HUNGARIAN_HPP
#define NOYA_HUNGARIAN_HPP 1
#include "noya/limit.hpp"
#include <cassert>
#include <tuple>
#include <vector>

namespace noya {

// @(res, match, X, Y)
template <typename T, bool MINIMIZE>
std::tuple<T, std::vector<int>, std::vector<T>, std::vector<T>>
hungarian(std::vector<std::vector<T>> &C) {
  int N = sz(C);
  int M = sz(C[0]);
  assert(N <= M);
  std::vector<std::vector<T>> A(N + 1, std::vector<T>(M + 1));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++)
      A[1 + i][1 + j] = (MINIMIZE ? 1 : -1) * C[i][j];
  ++N, ++M;

  std::vector<int> P(M), way(M);
  std::vector<T> X(N), Y(M);
  std::vector<T> minV;
  std::vector<bool> used;

  for (int i = 1; i < N; i++) {
    P[0] = i;
    minV.assign(M, infty<T>);
    used.assign(M, false);
    int j0 = 0;
    while (P[j0] != 0) {
      int i0 = P[j0], j1 = 0;
      used[j0] = true;
      T delta = infty<T>;
      for (int j = 1; j < M; j++) {
        if (used[j])
          continue;
        T curr = A[i0][j] - X[i0] - Y[j];
        if (curr < minV[j])
          minV[j] = curr, way[j] = j0;
        if (minV[j] < delta)
          delta = minV[j], j1 = j;
      }
      for (int j = 0; j < M; j++) {
        if (used[j])
          X[P[j]] += delta, Y[j] -= delta;
        else
          minV[j] -= delta;
      }
      j0 = j1;
    }
    do {
      P[j0] = P[way[j0]];
      j0 = way[j0];
    } while (j0 != 0);
  }
  T res = -Y[0];
  X.erase(X.begin());
  Y.erase(Y.begin());
  std::vector<int> match(N);
  for (int i = 0; i < M; i++)
    match[P[i]] = i;
  match.erase(match.begin());
  for (auto &&i : match)
    --i;
  if (!MINIMIZE)
    res = -res;
  return {res, match, X, Y};
}
} // namespace noya

#endif // NOYA_HUNGARIAN_HPP
