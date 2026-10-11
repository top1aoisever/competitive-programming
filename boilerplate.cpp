#include <iostream>
#include <cstdio>

void fileIO(const char* inputFile, const char* outputFile) {
    freopen(inputFile, "r", stdin);
    freopen(outputFile, "w", stdout);
}

void solve() {
    std::cout << "hello";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    int tests;
    std::cin >> tests;
    while (tests--) {
        solve();
    }
}
