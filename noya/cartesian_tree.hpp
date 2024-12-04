#ifndef NOYA_CARTESIAN_TREE_HPP
#define NOYA_CARTESIAN_TREE_HPP 1

// https://github.com/hos-lyric/libra/blob/master/data_structure/cartesian_tree.cpp
#include <assert.h>
#include <vector>

using std::vector;

// root: min (tie-break: left)
struct MinCartesianTree {
  int n, rt;
  vector<int> par, lef, rig;
  template <class T> void build(int n_, T *as) {
    assert(n_ >= 1);
    n = n_;
    rt = 0;
    par.assign(n, -1);
    lef.assign(n, -1);
    rig.assign(n, -1);
    int top = 0;
    vector<int> stack(n, 0);
    for (int u = 1; u < n; ++u) {
      if (as[stack[top]] > as[u]) { // >
        for (; top >= 1 && as[stack[top - 1]] > as[u]; --top) {
        } // >
        if (top == 0) {
          rt = par[lef[u] = stack[top]] = u;
        } else {
          par[lef[u] = stack[top]] = u;
          rig[par[u] = stack[top - 1]] = u;
        }
        stack[top] = u;
      } else {
        rig[par[u] = stack[top]] = u;
        stack[++top] = u;
      }
    }
  }
  template <class T> void build(const T &as) { build(as.size(), as.data()); }
};

// root: max (tie-break: left)
struct MaxCartesianTree {
  int n, rt;
  vector<int> par, lef, rig;
  template <class T> void build(int n_, T *as) {
    assert(n_ >= 1);
    n = n_;
    rt = 0;
    par.assign(n, -1);
    lef.assign(n, -1);
    rig.assign(n, -1);
    int top = 0;
    vector<int> stack(n, 0);
    for (int u = 1; u < n; ++u) {
      if (as[stack[top]] < as[u]) { // <
        for (; top >= 1 && as[stack[top - 1]] < as[u]; --top) {
        } // <
        if (top == 0) {
          rt = par[lef[u] = stack[top]] = u;
        } else {
          par[lef[u] = stack[top]] = u;
          rig[par[u] = stack[top - 1]] = u;
        }
        stack[top] = u;
      } else {
        rig[par[u] = stack[top]] = u;
        stack[++top] = u;
      }
    }
  }
  template <class T> void build(const T &as) { build(as.size(), as.data()); }
};

#endif // NOYA_CARTESIAN_TREE_HPP