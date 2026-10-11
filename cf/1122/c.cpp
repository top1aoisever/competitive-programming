#include <iostream>
#include <vector>
#include <cmath>

void solve() {
    int n;
    std::cin >> n;
    std::string s;
    std::cin >> s;
    std::vector<int> oneCnt(n);
    for (int i = n-1; i >= 0; i--) {
        if (s[i] == '1') oneCnt[i]++;
        if (i < n-1) oneCnt[i] += oneCnt[i+1];
    }
    if (s[0] == '1') {
        std::cout << n - 1 - oneCnt[1] << '\n';
        return;
    }

    int ans = n;
    int convertedToZero = 0;

    for (int i = 1; i < n-1; i++) {
        if (s[i] == '1') {
            ans = std::min(ans, convertedToZero + (n-i-1) - oneCnt[i+1]);
            convertedToZero++;
        }
    }
    ans = std::min(ans, convertedToZero);
    std::cout << ans << '\n';
}

int main() {
    int tests;
    std::cin >> tests;

    while (tests--) solve();
}
