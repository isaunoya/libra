#ifndef NOYA_ONLINEZALGO_HPP
#define NOYA_ONLINEZALGO_HPP 1

#include <vector>
namespace noya {
template <class S> struct online_z_algo {
  std::vector<std::vector<int>> memo;
  std::vector<int> z;
  std::vector<S> str;
  int p;
  online_z_algo() { p = 1; }

  std::vector<int> add(int i, S c) {
    str.push_back(c);
    int len = str.size();
    z.push_back(-1);
    memo.resize(1 + i);
    std::vector<int> end;

    if (len == 1) {
      return end;
    }

    if (str[0] != c) {
      z[i] = 0;
      end.push_back(i);
    }

    auto del = [&](int j) {
      z[j] = i - j;
      memo[i].push_back(j);
      end.push_back(j);
    };

    while (p <= i) {
      if (z[p] != -1) {
        p += 1;
      } else if (str[i - p] != str[i]) {
        del(p);
        p += 1;
      } else {
        break;
      }
    }
    if (p < i) {
      for (auto j : memo[i - p]) {
        del(j + p);
      }
    }
    return end;
  }

  int query(int i) {
    if (z[i] == -1) {
      return str.size() - i;
    }
    return z[i];
  }
};
} // namespace noya
#endif // NOYA_ONLINEZALGO_HPP