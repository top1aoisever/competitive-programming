#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
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
    i32 n, m;
    std::cin >> n >> m;

    std::map<i32, i32> carrots;
    std::vector<i32> ans(m+1, 0);

    for (int i = 0; i < n; i++) {
        i32 a;
        std::cin >> a;
        carrots[a]++;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    i32 tests;
    std::cin >> tests;
    while (tests--) {
        solve();
    }
}
