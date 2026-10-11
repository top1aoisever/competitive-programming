#include <iostream>
#include <cmath>

int main() {
  int t;
  std::cin >> t;
  while (t--) {
    long long x;
    std::cin >> x;
    long long digits = 1;
    long long pow10 = 10;
    while (x / pow10 >= 1) {
      digits++;
      pow10*=10;
    }
    std::cout << pow10 + 1 << '\n';
  }
}
