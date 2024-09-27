#ifndef NOYA_SPARSETABLE_HPP
#define NOYA_SPARSETABLE_HPP 1

#include <cassert>
#include <vector>

namespace noya {
template <class T, auto op, T e = T()> struct sparse_table {
  static int highest_bit(unsigned x) {
    return x == 0 ? -1 : 31 - __builtin_clz(x);
  }

  int n = 0;
  std::vector<std::vector<T>> ST;

  sparse_table(const std::vector<T> &values = {}) {
    if (!values.empty())
      build(values);
  }

  void build(const std::vector<T> &values) {
    n = int(values.size());
    int levels = highest_bit(n) + 1;
    ST.resize(levels);

    for (int k = 0; k < levels; k++)
      ST[k].resize(n - (1 << k) + 1);

    if (n > 0)
      ST[0] = values;

    for (int k = 1; k < levels; k++)
      for (int i = 0; i <= n - (1 << k); i++)
        ST[k][i] = op(ST[k - 1][i], ST[k - 1][i + (1 << (k - 1))]);
  }
  T prod(int l, int r) const {
    assert(0 <= l && l <= r && r <= n);
    if (l == r) {
      return e;
    }
    int L = highest_bit(r - l);
    return op(ST[L][l], ST[L][r - (1 << L)]);
  }
};
} // namespace noya

#endif // NOYA_SPARSETABLE_HPP