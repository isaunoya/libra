#ifndef NOYA_DIAM_HPP
#define NOYA_DIAM_HPP 1

#include <noya/fastlca.hpp>

#include <algorithm>
#include <array>
#include <utility>

namespace noya {
// don't forget to build.
struct dist {
  fastlca *lca = nullptr;

  dist() = default;
  explicit dist(fastlca &l) { build(l); }

  void build(fastlca &l) { lca = &l; }

  int64_t operator()(int a, int b) const {
    assert(lca != nullptr);
    return lca->distance(a, b);
  }
} tree_distance;

using diam = std::pair<int64_t, std::array<int, 2>>;
const diam diam_E = {-1, {-1, -1}};
diam diam_e() { return diam_E; }

diam diam_op(diam a, diam b) {
  if (a == diam_E) {
    return b;
  } else if (b == diam_E) {
    return a;
  }
  diam c = std::max(a, b);
  for (auto x : a.second) {
    for (auto y : b.second) {
      int64_t d = tree_distance(x, y);
      if (d > c.first) {
        c = std::make_pair(d, std::array<int, 2>{x, y});
      }
    }
  }
  return c;
}

} // namespace noya

#endif // NOYA_DIAM_HPP