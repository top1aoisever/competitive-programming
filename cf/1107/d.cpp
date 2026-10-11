#include <iostream>
#include <vector>

int main() {
  int tests;
  std::cin >> tests;
  while (tests--) {
    int n;
    std::cin >> n;
    std::vector<long long> a(n), b(n);
    for (long long &i : a) std::cin >> i;
    for (long long &i : b) std::cin >> i;
    bool flag = true;
    for (int i = 0; i < n-1; ++i) {
      if (a[i] > b[i]) {
        flag = false;
        break;
      }
      a[i+1] -= (b[i] - a[i]);
    }
    if (flag) flag = (a[n-1] <= b[n-1]);
    std::cout << (flag ? "YES" : "NO") << '\n';
  }
}
