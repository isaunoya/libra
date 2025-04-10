#ifndef NOYA_LIMIT_HPP
#define NOYA_LIMIT_HPP 1

#include <cstdint>
using ll = int64_t;
using ull = uint64_t;
namespace noya {
template <class T> constexpr T infty = 0;
template <> constexpr int infty<int> = 1010000000;
template <> constexpr ll infty<ll> = 2020000000000000000;
template <> constexpr unsigned infty<unsigned> = infty<int>;
template <>
constexpr ull infty<ull> = infty<ll>;
template <>
constexpr __int128 infty<__int128> =
    __int128(infty<ll>) * 2000000000000000000;
template <> constexpr double infty<double> = infty<ll>;
template <> constexpr long double infty<long double> = infty<ll>;
} // namespace noya

#endif // NOYA_LIMIT_HPP