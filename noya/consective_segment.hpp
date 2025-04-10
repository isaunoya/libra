#ifndef NOYA_CONSECTIVE_SEGMENT_HPP
#define NOYA_CONSECTIVE_SEGMENT_HPP 1

#include <array>
#include <map>
#include <vector>

template <class T> struct consective_segment {
  std::map<int, int> mp;
  consective_segment() {}
  consective_segment(int N, int v = 0) { mp[N] = v; }
  void split(int x) {
    auto it = mp.lower_bound(x);
    mp[x] = it->second;
  }

  // assign [l, r) = v
  // return the origin segments @([l, r), v)
  std::vector<std::tuple<int, int, T>> assign(int l, int r, T v) {
    split(l);
    split(r);
    auto it = mp.find(l);
    T t = mp[l];
    std::vector<std::tuple<int, int, T>> segments;
    while (it->first != r) {
      auto nx = next(it);
      segments.push_back({it->first, nx->first, nx->second});
      it = mp.erase(it);
    }
    mp[l] = t;
    mp[r] = v;
    return segments;
  }
};

#endif // NOYA_CONSECTIVE_SEGMENT_HPP