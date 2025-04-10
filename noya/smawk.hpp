#ifndef NOYA_SMAWK_HPP
#define NOYA_SMAWK_HPP 1

// https://noshi91.github.io/Library/algorithm/smawk.cpp.html
#include <functional>
#include <numeric>
#include <vector>

template <class Select>
std::vector<int> smawk(const int row_size, const int col_size,
                       const Select &select) {
  const std::function<std::vector<int>(const std::vector<int> &,
                                       const std::vector<int> &)>
      solve = [&](const std::vector<int> &row,
                  const std::vector<int> &col) -> std::vector<int> {
    const int n = int(row.size());
    if (n == 0)
      return {};
    std::vector<int> c2;
    for (const int i : col) {
      while (!c2.empty() && select(row[c2.size() - 1], c2.back(), i))
        c2.pop_back();
      if (c2.size() < n)
        c2.push_back(i);
    }
    std::vector<int> r2;
    for (int i = 1; i < n; i += 2)
      r2.push_back(row[i]);
    const std::vector<int> a2 = solve(r2, c2);
    std::vector<int> ans(n);
    for (int i = 0; i != a2.size(); i += 1)
      ans[i * 2 + 1] = a2[i];
    int j = 0;
    for (int i = 0; i < n; i += 2) {
      ans[i] = c2[j];
      const int end = i + 1 == n ? c2.back() : ans[i + 1];
      while (c2[j] != end) {
        j += 1;
        if (select(row[i], ans[i], c2[j]))
          ans[i] = c2[j];
      }
    }
    return ans;
  };
  std::vector<int> row(row_size);
  std::iota(row.begin(), row.end(), 0);
  std::vector<int> col(col_size);
  std::iota(col.begin(), col.end(), 0);
  return solve(row, col);
}

#endif // NOYA_SMAWK_HPP