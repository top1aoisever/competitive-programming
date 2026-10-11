#include <iostream>
#include <vector>
#include <cmath>

long long ans = 0;

void dfs(
  int n,
  int u,
  int p,
  std::vector<int> &subSize,
  const std::vector<std::vector<int>> &adj,
  const std::vector<bool> &isPerfect
  ) {

  std::vector<long long> subTrees;
  for (const int &v : adj[u]) {
    if (v == p) continue;
    dfs(n, v, u, subSize, adj, isPerfect);
    subTrees.push_back(subSize[v]);
    subSize[u] += subSize[v];
  }

  subTrees.push_back(n - subSize[u]);

  if (!isPerfect[u]) return;
  long long sum = 0;
  long long pair = 0;
  long long triple = 0;

  for (const long long &s : subTrees) {
    triple += pair * s;
    pair += sum * s;
    sum += s;
  }

  ans += pair;
  ans += triple;

  return;
}


void solve() {
  ans = 0;

  int n;
  std::cin >> n;
  std::vector<bool> isPerfect(n, false);

  for (int i = 0; i < n; ++i) {
    int a;
    std::cin >> a;
    int sqrt_a = std::sqrt(a);
    isPerfect[i] = (sqrt_a * sqrt_a == a);
  }

  std::vector<std::vector<int>> adj(n);
  std::vector<int> subSize(n, 1);

  for (int i = 0; i < n-1; ++i) {
    int u, v;
    std::cin >> u >> v;
    adj[--u].push_back(--v);
    adj[v].push_back(u);
  }

  dfs(n, 0, -1, subSize, adj, isPerfect);

  std::cout << ans << '\n';

}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  int tests;
  std::cin >> tests;
  while (tests--) {
    solve();
  }
}
