#ifndef NOYA_TREAP_HPP
#define NOYA_TREAP_HPP 1

#include "noya/rnd.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

namespace noya {
#if __cplusplus >= 201703L

// sequence treap
template <class S, auto op, auto e, class F, auto mapping, auto composition,
          auto id>
struct treap {
  static_assert(std::is_convertible_v<decltype(op), std::function<S(S, S)>>,
                "op must work as S(S, S)");
  static_assert(std::is_convertible_v<decltype(e), std::function<S()>>,
                "e must work as S()");
  static_assert(
      std::is_convertible_v<decltype(mapping), std::function<S(F, S)>>,
      "mapping must work as S(F, S)");
  static_assert(
      std::is_convertible_v<decltype(composition), std::function<F(F, F)>>,
      "composition must work as F(F, F)");
  static_assert(std::is_convertible_v<decltype(id), std::function<F()>>,
                "id must work as F()");

#else

template <class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S),
          F (*composition)(F, F), F (*id)()>
struct treap {
#endif
  std::vector<S> values;
  std::vector<F> tag;
  // std::vector<ull> rng;

  void push_back(S x) {}

  void apply() {}
};
} // namespace noya

#endif // NOYA_TREAP_HPP