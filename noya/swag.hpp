#ifndef NOYA_SWAG_HPP
#define NOYA_SWAG_HPP 1

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

namespace noya {
#if __cplusplus >= 201703L

template <class S, auto op, auto e> struct swag {
  static_assert(std::is_convertible_v<decltype(op), std::function<S(S, S)>>,
                "op must work as S(S, S)");
  static_assert(std::is_convertible_v<decltype(e), std::function<S()>>,
                "e must work as S()");

#else

template <class S, S (*op)(S, S), S (*e)()> struct swag {

#endif
  int sz = 0;
  std::vector<S> dat;
  std::vector<S> sum_l;
  S sum_r;

  swag() {
    sum_l = {e()};
    sum_r = e();
  }

  int size() { return sz; }
  void push(S x) {
    ++sz;
    sum_r = op(sum_r, x);
    dat.push_back(x);
  }

  void pop() {
    assert(0 < sz);
    --sz;
    sum_l.pop_back();
    if ((int)sum_l.size() == 0) {
      sum_l = {e()};
      sum_r = e();
      while ((int)dat.size() > 1) {
        sum_l.push_back(op(dat.back(), sum_l.back()));
        dat.pop_back();
      }
      dat.pop_back();
    }
  }

  S lprod() { return sum_l.back(); }
  S rprod() { return sum_r; }

  S prod() { return op(sum_l.back(), sum_r); }
};
} // namespace noya

#endif // NOYA_SWAG_HPP