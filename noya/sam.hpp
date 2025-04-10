#ifndef NOYA_SAM_HPP
#define NOYA_SAM_HPP 1

#include <array>
#include <map>
#include <vector>
#include <cassert>

namespace noya {
template <int sigma = 26> struct suffix_automaton {
  struct Node {
    std::array<int, sigma> next;
    int link;
    int len;

    Node(int link, int len) : link(link), len(len) {
      std::fill(next.begin(), next.end(), -1);
    }
  };

  std::vector<Node> nodes;
  int last = 0;

  suffix_automaton() {
    nodes.push_back(Node(-1, 0));
    last = 0;
  }

  // https://maspypy.github.io/library/string/suffix_automaton.hpp
  void extend(int c) {
    assert(0 <= c && c < sigma);
    int new_node = int(nodes.size());
    nodes.push_back(Node(-1, nodes[last].len + 1));
    int p = last;
    while (p != -1 && nodes[p].next[c] == -1) {
      nodes[p].next[c] = new_node;
      p = nodes[p].link;
    }
    int q = (p == -1 ? 0 : nodes[p].next[c]);
    if (p == -1 || nodes[p].len + 1 == nodes[q].len) {
      nodes[new_node].link = q;
    } else {
      int new_q = int(nodes.size());
      nodes.push_back(Node(nodes[q].link, nodes[p].len + 1));
      nodes.back().next = nodes[q].next;
      nodes[q].link = new_q;
      nodes[new_node].link = new_q;
      while (p != -1 && nodes[p].next[c] == q) {
        nodes[p].next[c] = new_q;
        p = nodes[p].link;
      }
    }
    last = new_node;
  }

  std::vector<std::vector<int>> get_tree() const {
    int n = int(nodes.size());
    std::vector<std::vector<int>> g(n);
    for (int i = 1; i < n; i++) {
      g[nodes[i].link].push_back(i);
    }
    return g;
  }

  long long count_substring_at(int i) const {
    if (i == 0) {
      return 0;
    } else {
      return nodes[i].len - nodes[nodes[i].link].len;
    }
  };
  long long count_substring() const {
    long long ans = 0;
    int n = int(nodes.size());
    for (int i = 1; i < n; i++) {
      ans += count_substring_at(i);
    }
    return ans;
  }
};

} // namespace noya

#endif // NOYA_SAM_HPP