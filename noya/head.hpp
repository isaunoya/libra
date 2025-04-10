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
template <typename T, typename T2> bool cmin(T &x, const T2 &y) {
  return (x > y ? x = y, 1 : 0);
}
template <typename T, typename T2> bool cmax(T &x, const T2 &y) {
  return (x < y ? x = y, 1 : 0);
}

#include "noya/limit.hpp"

using vi = vector<int>;
using vl = vector<ll>;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
template <class T> using vc = vector<T>;
const int INF = noya::infty<int>;
const ll LNF = noya::infty<ll>;
#define sz(x) int((x).size())
#define all(x) begin(x), end(x)
#define fi first
#define se second

namespace noya {

template <class T> vc<pair<T, int>> compress(const vc<T> &S) {
  int N = sz(S);
  vc<pair<T, int>> ret;
  for (int i = 0, j = 0; i < N; i = j) {
    while (j < N && S[i] == S[j])
      j++;
    ret.push_back({S[i], j - i});
  }
  return ret;
}

vc<pii> compress(const string &S) { return compress(vi(S.begin(), S.end())); }

} // namespace noya

#endif // NOYA_HEAD_HPP