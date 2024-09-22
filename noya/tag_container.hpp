#ifndef NOYA_TAG_CONTAINER_HPP
#define NOYA_TAG_CONTAINER_HPP 1

#include "noya/hashmap.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <map>
#include <vector>

namespace noya {
template <class K, class V, auto op> struct tag_HashMap {
  HashMap<K, V> S;
  V tag = V();
  // value indicates the true value
  void insert(K key, V value) {
    auto it = S.find(key);
    if (it == S.end()) {
      S[key] = value - tag;
    } else {
      S[key] = op(it->second, value - tag);
    }
  }

  void erase(K key) { S.erase(key); }
  int size() const { return (int)S.size(); }

  void join(tag_HashMap<K, V, op> &other) {
    if (size() > other.size()) {
      for (auto &[key, value] : other.S) {
        insert(key, value + other.tag);
      }
    } else {
      S.swap(other.S);
      swap(tag, other.tag);
      for (auto &[key, value] : other.S) {
        insert(key, value + other.tag);
      }
    }
  }

  void add(V x) { tag += x; }

  // return the true value
  V operator[](const K &x) {
    if (S.find(x) == S.end()) {
      return V();
    } else {
      V value = S[x];
      return value + tag;
    }
  }
};

} // namespace noya

#endif // NOYA_TAG_CONTAINER_HPP