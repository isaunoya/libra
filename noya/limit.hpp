#ifndef NOYA_LIMIT_HPP
#define NOYA_LIMIT_HPP 1

namespace noya {
template <class T> constexpr T infty = 0;
template <> constexpr int infty<int> = 1010000000;
template <> constexpr long long infty<long long> = 2020000000000000000;
template <> constexpr unsigned infty<unsigned> = infty<int>;
template <>
constexpr unsigned long long infty<unsigned long long> = infty<long long>;
template <>
constexpr __int128 infty<__int128> =
    __int128(infty<long long>) * 2000000000000000000;
template <> constexpr double infty<double> = infty<long long>;
template <> constexpr long double infty<long double> = infty<long long>;
} // namespace noya

#endif // NOYA_LIMIT_HPP