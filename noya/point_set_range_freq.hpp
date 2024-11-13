#ifndef NOYA_POINT_SET_RANGE_FREQUENCY_HPP
#define NOYA_POINT_SET_RANGE_FREQUENCY_HPP 1

#include <algorithm>
#include <vector>
namespace noya {

template <class T> struct point_set_range_frequency {
  point_set_range_frequency() {}

  const int B = 4000;
  int N;
  std::vector<T> A;
  point_set_range_frequency(std::vector<T> &_A) {
    A = _A;
    if (!A.empty())
      build();
  }

  std::vector<std::pair<int, T>> point_set;
  void set(int x, T v) {
    assert(0 <= x && x < N);
    for (auto &[p, q] : point_set) {
      if (p == x) {
        q = v;
        return;
      }
    }
    point_set.push_back({x, v});
    if (int(point_set.size()) > B) {
      build();
    }
  }

  std::vector<std::vector<int>> pos;
  std::vector<T> Xs;

  // frequency [l, r)
  int query(int l, int r, T x) {
    assert(0 <= l && l <= r && r <= N);
    int ans = 0;
    if (std::binary_search(Xs.begin(), Xs.end(), x)) {
      int a = std::lower_bound(Xs.begin(), Xs.end(), x) - Xs.begin();
      ans += std::lower_bound(pos[a].begin(), pos[a].end(), r) -
             std::lower_bound(pos[a].begin(), pos[a].end(), l);
    }
    for (auto &[p, q] : point_set) {
      if (l <= p && p < r) {
        if (A[p] == x) {
          ans--;
        }
        if (q == x) {
          ans++;
        }
      }
    }
    return ans;
  }

  void build() {
    if (!point_set.empty()) {
      for (auto &[x, v] : point_set) {
        A[x] = v;
      }
      point_set.clear();
    }

    Xs.clear();
    for (auto &a : A)
      Xs.push_back(a);
    std::sort(Xs.begin(), Xs.end());
    Xs.erase(std::unique(Xs.begin(), Xs.end()), Xs.end());

    N = int(A.size());
    pos.assign(Xs.size(), {});
    for (int i = 0; i < N; i++) {
      auto a = A[i];
      a = std::lower_bound(Xs.begin(), Xs.end(), a) - Xs.begin();
      pos[a].push_back(i);
    }
  }
};
} // namespace noya

#endif // NOYA_POINT_SET_RANGE_FREQUENCY_HPP