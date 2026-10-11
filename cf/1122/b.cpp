#include <iostream>
#include <cmath>

void solve() {
    int alice, bob, pile;
    std::cin >> alice >> bob >> pile;

    if (alice >= bob) std::cout << alice + pile - bob;
    else {
        int aliceFull = alice + pile;
        if (aliceFull <= bob) std::cout << bob - alice;
        else {
            if (bob - alice <= aliceFull - bob) std::cout << aliceFull - bob;
            else std::cout << bob - alice;
        }
    }
    std::cout << '\n';
}

int main() {
    int tests;
    std::cin >> tests;

    while (tests--) solve();
}
