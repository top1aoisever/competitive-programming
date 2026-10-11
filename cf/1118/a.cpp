#include <iostream>

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    int t;
    std::cin >> t;
    while (t--) {
        int n;
        std::cin >> n;
        int start, end;
        for (int i = 0; i < n; i++) {
            int a;
            std::cin >> a;
            if (i == 0) start = a;
            if (i == n-1) end = a;
        }
        std::cout << gcd(start, end) << '\n';
    }
}
