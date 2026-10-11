#include <iostream>
#include <cmath>

void solve() {
    int n;
    std::cin >> n;
    int a, b, c;
    std::cin >> a >> b >> c;
    std::cout << n - std::min(a, std::min(b, c)) << '\n';
}

int main() {
    int t;
    std::cin >> t;
    while (t--) solve();
}
