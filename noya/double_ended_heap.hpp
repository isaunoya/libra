#ifndef NOYA_DOUBLE_ENDED_HEAP_HPP
#define NOYA_DOUBLE_ENDED_HEAP_HPP 1

#include <algorithm>
#include <functional>
#include <queue>
#include <vector>

namespace noya {
template <class T, class C> struct removable_heap {
  std::priority_queue<T, std::vector<T>, C> p, q;
  void push(T x) { p.emplace(x); }
  void pop(T x) { q.emplace(x); }

  T top() {
    while (!p.empty() && !q.empty() && p.top() == q.top()) {
      p.pop();
      q.pop();
    }
    assert(!p.empty());
    return p.top();
  }
};

template <class T> struct double_ended_heap {
  removable_heap<T, std::greater<>> min_heap;
  removable_heap<T, std::less<>> max_heap;

  void push(T x) {
    min_heap.push(x);
    max_heap.push(x);
  }
  void pop(T x) {
    min_heap.pop(x);
    max_heap.pop(x);
  }

  T get_min() { return min_heap.top(); }
  T get_max() { return max_heap.top(); }
};
} // namespace noya

#endif // NOYA_DOUBLE_ENDED_HEAP_HPP