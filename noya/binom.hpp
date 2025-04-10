#ifndef NOYA_BINOM_HPP
#define NOYA_BINOM_HPP 1

#include "atcoder/modint.hpp"

#include <vector>

template <class T> struct binom {
  std::vector<T> fac, ifac;
  binom() {}
  binom(int n) { prepare(n); }
  
  void prepare(int n) {
    if (fac.empty()) {
      fac = {1};
      ifac = {1};
    }

    for (int i = int(fac.size()); i <= n; i++) {
      fac.push_back(fac[i - 1] * i);
      ifac.push_back(fac[i].inv());
    }
  }

  T fact(int n) {
    if (n >= int(fac.size()))
      prepare(n * 2);
    return fac[n];
  }

  T ifact(int n) {
    if (n >= int(ifac.size()))
      prepare(n * 2);
    return ifac[n];
  }

  T C(int n, int m) {
    assert(0 <= m && m <= n);
    return fact(n) * ifact(m) * ifact(n - m);
  }
};

#endif // NOYA_BINOM_HPP