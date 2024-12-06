#ifndef NOYA_GCD_MINTANENANCE_HPP
#define NOYA_GCD_MINTANENANCE_HPP 1

#include <algorithm>
#include <numeric>
#include <vector>
namespace noya {

// returns r for each starting position l.
template <class T, auto op>
std::vector<std::vector<int>> get_suffix(const std::vector<T> &A) {
  int N = int(A.size());
  std::vector<std::vector<int>> position(N);
  std::vector<std::pair<T, int>> cur;
  for (int i = N - 1; i >= 0; i--) {
    T x = A[i];
    if (!cur.empty()) {
      std::vector<std::pair<T, int>> new_cur;
      std::reverse(cur.begin(), cur.end());
      T g = x;
      for (const auto &[value, pos] : cur) {
        g = op(g, value);
        if (new_cur.empty() || new_cur.back().first != g) {
          new_cur.push_back({g, pos});
        }
      }
      cur.swap(new_cur);
      std::reverse(cur.begin(), cur.end());
    }
    cur.push_back({x, i + 1});
    for (const auto &[value, pos] : cur) {
      position[i].push_back(pos);
    }
  }
  return position;
};

// returns (r, prod(l, r)) for each starting position l.
template <class T, auto op>
std::vector<std::vector<std::pair<int, T>>>
get_suffix_values(const std::vector<T> &A) {
  int N = int(A.size());
  std::vector<std::vector<int>> values(N);
  std::vector<std::pair<int, T>> cur;
  for (int i = N - 1; i >= 0; i--) {
    T x = A[i];
    if (!cur.empty()) {
      std::vector<std::pair<int, T>> new_cur;
      std::reverse(cur.begin(), cur.end());
      T g = x;
      for (const auto &[pos, values] : cur) {
        g = op(values, g);
        if (new_cur.empty() || new_cur.back().second != g) {
          new_cur.push_back({pos, g});
        }
      }
      cur.swap(new_cur);
      std::reverse(cur.begin(), cur.end());
    }
    cur.push_back({i + 1, x});
    values[i] = cur;
  }
  return values;
};

} // namespace noya

#endif // NOYA_GCD_MINTANANCE_HPP