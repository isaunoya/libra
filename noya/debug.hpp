#ifndef NOYA_DEBUG_HPP
#define NOYA_DEBUG_HPP 1

#include "atcoder/modint.hpp"
#include "noya/bitset.hpp"

#include <iostream>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

template <class T, size_t size = std::tuple_size<T>::value>
std::string to_debug(T, std::string s = "")
  requires(not std::ranges::range<T>);
std::string to_debug(auto x)
  requires requires(std::ostream &os) { os << x; }
{
  return static_cast<std::ostringstream>(std::ostringstream() << x).str();
}
template <typename T>
std::string to_debug(T x)
  requires std::is_base_of_v<atcoder::internal::modint_base, T>
{
  return std::to_string(x.val());
}

std::string to_debug(noya::Bitset x)
{
  return noya::to_string(x);
}

std::string to_debug(std::ranges::range auto x, std::string s = "")
  requires(not std::is_same_v<decltype(x), std::string>)
{
  for (auto xi : x) {
    s += ", " + to_debug(xi);
  }
  return "[" + s.substr(s.empty() ? 0 : 2) + "]";
}
template <class T, size_t size>
std::string to_debug(T x, std::string s)
  requires(not std::ranges::range<T>)
{
  [&]<size_t... I>(std::index_sequence<I...>) {
    ((s += ", " + to_debug(get<I>(x))), ...);
  }(std::make_index_sequence<size>());
  return "(" + s.substr(s.empty() ? 0 : 2) + ")";
}
#define debug(...)                                                             \
  std::cerr << "[" #__VA_ARGS__ "] = " << to_debug(std::tuple(__VA_ARGS__)) << "\n"

#endif // NOYA_DEBUG_HPP