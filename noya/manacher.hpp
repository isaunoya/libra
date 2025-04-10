#ifndef NOYA_MANACHER_HPP
#define NOYA_MANACHER_HPP 1

#include <cassert>
#include <string>
#include <vector>

template <class T> std::vector<int> manacher(const std::vector<T> &s) {
  int n = int(s.size());
  if (n == 0)
    return {};
  std::vector<T> t{-2, -1};
  for (auto &a : s) {
    assert(a >= 0);
    t.push_back(a);
    t.push_back(-1);
  }
  int N = int(t.size());
  std::vector<int> p(N);
  int m = -1, r = -1;
  for (int i = 0; i < N; i++) {
    if (i <= r) {
      p[i] = std::min(p[m * 2 - i], r - i + 1);
    }
    while (i - p[i] >= 0 && i + p[i] < N && t[i + p[i]] == t[i - p[i]]) {
      ++p[i];
    }
    if (i + p[i] > r) {
      r = i + p[i] - 1;
      m = i;
    }
    --p[i];
  }
  std::vector<T>().swap(t);
  return std::vector<int>(p.begin() + 2, p.end() - 1);
}

std::vector<int> manacher(const std::string &s) {
  int n = int(s.size());
  std::vector<int> s2(n);
  for (int i = 0; i < n; i++) {
    s2[i] = s[i];
  }
  return manacher(s2);
}

#endif // NOYA_MANACHER_HPP