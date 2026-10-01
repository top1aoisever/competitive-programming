#include <bits/stdc++.h>

int main() {
  int t;
  std::cin >> t;
  while (t--) {
    int n;
    std::string s;
    std::cin >> n >> s;
    int cnt = 0;
    for (int i = 0; i < n-1; ++i) {
      if (s[i] != s[i+1]) ++cnt;
    }
    std::cout << (cnt == 1 ? 2 : 1) << '\n';
  }

  return 0;
}
