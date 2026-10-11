#include <iostream>

int main() {
  int t;
  std::cin >> t;
  while (t--) {
    int a, b;
    std::cin >> a >> b;
    if (a % b == 0) std::cout << "YES";
    else std::cout << "NO";
    std::cout << '\n';
  }
}
