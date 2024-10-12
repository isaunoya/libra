#ifndef NOYA_HEAD_HPP
#define NOYA_HEAD_HPP 1

#if defined(local)
#include "./noya/debug.hpp"
#else
#define debug(...) 42
#endif

#include "bits/stdc++.h"

using namespace std;
#define rep1(a) for (int i = 0; i < a; i++)
#define rep2(i, a) for (int i = 0; i < a; i++)
#define rep3(i, a, b) for (int i = a; i < b; i++)
#define rep4(i, a, b, c) for (int i = a; i < b; i += c)
#define overload4(a, b, c, d, e, ...) e
#define rep(...) overload4(__VA_ARGS__, rep4, rep3, rep2, rep1)(__VA_ARGS__)
#define pb emplace_back
template <typename T, typename T2> void cmin(T &x, const T2 &y) {
  x = x < y ? x : y;
}
template <typename T, typename T2> void cmax(T &x, const T2 &y) {
  x = x > y ? x : y;
}
using ll = int64_t;
using vi = vector<int>;
using vl = vector<ll>;
using ull = uint64_t;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
template <class T> using vc = vector<T>;
const int INF = 1000000000;
const ll LNF = 1000000000000000000;
#define sz(x) int((x).size())
#define all(x) begin(x), end(x)
#define fi first
#define se second

namespace noya {}

#endif // NOYA_HEAD_HPP