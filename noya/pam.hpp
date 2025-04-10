#ifndef NOYA_PAM_HPP
#define NOYA_PAM_HPP 1

template <int PAM_NODES> struct PAM {
  PAM() { init(); }
  int fail[PAM_NODES + 2];
  int len[PAM_NODES + 2];
  int cnt[PAM_NODES + 2];
  int num[PAM_NODES + 2];
  int s[PAM_NODES + 2];
  struct Edge {
    int v, nxt, w;
  } e[PAM_NODES + 2];
  int head[PAM_NODES + 2];
  int ec;
  void link(int u, int v, int w) {
    e[++ec] = {v, head[u], w};
    head[u] = ec;
  }
  int n, p, last;
  void init() {
    n = 0;
    p = 0;
    ec = 0;
    s[0] = -1;
    newnode(0);
    newnode(-1);
    last = 0;
    fail[0] = 1;
  }
  int newnode(int l) {
    head[p] = 0;
    cnt[p] = 0;
    num[p] = 0;
    len[p] = l;
    return p++;
  }
  int getfail(int x) {
    while (s[n - len[x] - 1] != s[n]) {
      x = fail[x];
    }
    return x;
  }
  int find(int cur, int c) {
    for (int i = head[cur]; i; i = e[i].nxt) {
      if (e[i].w == c) {
        return e[i].v;
      }
    }
    return 0;
  }
  void insert(int c) {
    s[++n] = c;
    int cur = getfail(last);
    if (!find(cur, c)) {
      int now = newnode(len[cur] + 2);
      fail[now] = find(getfail(fail[cur]), c);
      link(cur, now, c);
      num[now] = num[fail[now]] + 1;
    }
    last = find(cur, c);
    cnt[last]++;
  }
};

#endif // NOYA_PAM_HPP