#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

typedef long long i64;
typedef unsigned long long u64;

typedef int i32;
typedef unsigned int u32;

void fileIO(const char* inputFile, const char* outputFile) {
    freopen(inputFile, "r", stdin);
    freopen(outputFile, "w", stdout);
}

void solve() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> carrots(n);
    for (int &i : carrots) std::cin >> i;
    std::sort(carrots.begin(), carrots.end(), std::greater<int>());
    std::vector<int> maxCuts(m+1, 0);
    for (const int carrot : carrots) {
        maxCuts[carrot]++;
        if (!(carrot & 1)) {
            maxCuts[carrot/2]++;
            maxCuts[carrot/2 - 1]--;
        }
    }
    for (int i = m-1; i > 0; i--) {
        maxCuts[i] += maxCuts[i+1];
    }

    std::cout << *std::max_element(maxCuts.begin(), maxCuts.end()) << '\n';
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
