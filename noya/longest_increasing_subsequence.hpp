#ifndef NOYA_LONGEST_INCREASING_SUBSEQUENCE_HPP
#define NOYA_LONGEST_INCREASING_SUBSEQUENCE_HPP 1

#include <algorithm>
#include <cassert>
#include <vector>

// return the indice
template<class T>
std::vector<int> longest_increasing_subsequence(const std::vector<T> &A) {
  if (A.empty())
    return {};
  std::vector<T> B;
  std::vector<int> indice;
  int N = int(A.size());
  std::vector<int> pre(N, -1);
  for (int i = 0; i < N; i++) {
    T a = A[i];
    int j = std::lower_bound(B.begin(), B.end(), a) - B.begin();
    if (j == int(B.size())) {
      indice.push_back(i);
      B.push_back(a);
    } else {
      indice[j] = i;
      B[j] = a;
    }
    if (j > 0) {
      pre[i] = indice[j - 1];
    }
  }
  std::vector<int> ans;
  for (int cur = indice.back(); cur != -1; cur = pre[cur])
    ans.push_back(cur);

  std::reverse(ans.begin(), ans.end());
  return ans;
}

#endif // NOYA_LONGEST_INCREASING_SUBSEQUENCE_HPP