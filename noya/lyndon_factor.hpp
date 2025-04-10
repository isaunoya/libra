#ifndef NOYA_LYNDON_FACTOR_HPP
#define NOYA_LYNDON_FACTOR_HPP 1

#include <vector>
template <class T> std::vector<int> lyndon(const std::vector<T> &s) {
  int n = int(s.size());
  std::vector<int> res{0};
  for (int i = 0; i < n;) {
    int j = i + 1, k = i;
    for (; j < n && s[k] <= s[j]; j += 1)
      k = s[k] < s[j] ? i : k + 1;
    while (i <= k)
      res.push_back(i += j - k);
  }
  return res;
}
std::vector<int> lyndon(const std::string &s) {
  std::vector<char> a(s.begin(), s.end());
  return lyndon(a);
}

#endif // NOYA_LYNDON_FACTOR_HPP