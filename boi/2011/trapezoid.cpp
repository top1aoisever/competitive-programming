#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <utility>
#include <queue>

const int g_k_MaxCoordinate = 1e9;

const int g_k_Mod = 30013;

int compress(const std::vector<int>& v, const int& x) {
    return std::lower_bound(v.begin(), v.end(), x) - v.begin();
}

std::pair<int, int> operator+(const std::pair<int, int>& a, const std::pair<int, int>& b) {
    if (a.first > b.first) return a;
    if (b.first > a.first) return b;
    return std::make_pair(a.first, (a.second + b.second) % g_k_Mod);
}

std::pair<int, int> defaultValue() {
    return std::make_pair(0, 1);
}

class SegPURS {
    private:
        int n;
        std::vector<std::pair<int, int>> tree;

        void upd(const int& id, const int& l, const int& r, const int& idx, const std::pair<int, int>& v) {
            if (l == r && l == idx) {
                tree[id] = v;
                return;
            }

            int m = (l + r) >> 1;
            if (idx > m) upd(id<<1|1, m+1, r, idx, v);
            else upd(id<<1, l, m, idx, v);

            tree[id] = tree[id<<1] + tree[id<<1|1];
            // std::cout << l << ' ' << r << ' ' << tree[id].first << ' ' << tree[id].second << '\n';
        }

        std::pair<int, int> query(const int& id, const int& l, const int& r, const int& ql, const int& qr) {
            if (l >= ql && r <= qr) return tree[id];

            if (ql > r || qr < l) return std::make_pair(0, 0);

            int m = (l + r) >> 1;
            return query(id<<1, l, m, ql, qr) + query(id<<1|1, m+1, r, ql, qr);
        }

    public:
        SegPURS(const int& n): n(n) {
            tree.assign(n << 2, std::make_pair(0, 0));
            // tree[1] = std::make_pair(0, 1);
        }

        void upd(const int& idx, const std::pair<int, int>& v) {
            upd(1, 0, n-1, idx, v);
        }

        std::pair<int, int> query(const int &ql, const int& qr) {
            std::pair<int, int> ans = query(1, 0, n-1, ql, qr);
            if (ans.second == 0) ans.second = 1;
            return ans;
        }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    int n;
    std::cin >> n;

    std::vector<std::array<int, 4>> trapezoids;
    std::vector<int> compressUpper;
    std::vector<int> compressLower;

    for (int i = 0; i < n; i++) {
        int a, b, c, d;
        std::cin >> a >> b >> c >> d;

        compressUpper.push_back(a);
        compressUpper.push_back(b);
        compressLower.push_back(c);
        compressLower.push_back(d);

        trapezoids.push_back({a, b, c, d});
    }

    if (n == 1) {
        std::cout << 1 << ' ' << 1 << '\n';
        return 0;
    }

    std::sort(compressUpper.begin(), compressUpper.end());
    std::sort(compressLower.begin(), compressLower.end());
    compressUpper.erase(std::unique(compressUpper.begin(), compressUpper.end()), compressUpper.end());
    compressLower.erase(std::unique(compressLower.begin(), compressLower.end()), compressLower.end());

    for (int i = 0; i < n; i++) {
        std::array<int, 4>& t = trapezoids[i];
        t[0] = compress(compressUpper, t[0]);
        t[1] = compress(compressUpper, t[1]);
        t[2] = compress(compressLower, t[2]);
        t[3] = compress(compressLower, t[3]);
    }

    // dp[i] = max independent set that ends at i and its count

    std::vector<std::pair<int, int>> dp(n, std::make_pair(0, 0));
    std::pair<int, int> ans = defaultValue();

    std::sort(trapezoids.begin(), trapezoids.end());

    std::priority_queue<std::pair<int, int>> pq;

    SegPURS sgt(compressLower.size());

    // duh
    dp[0] = std::make_pair(1, 1);

    pq.push(std::make_pair(-trapezoids[0][1], 0));
    //
    for (int i = 1; i < n; i++) {
        const std::array<int, 4>& t = trapezoids[i];
        while (!pq.empty() && -pq.top().first < t[0]) {
            std::pair<int, int> p = pq.top();
            pq.pop();
            int updIdx = p.second;
            // TODO: make the tree
            // std::cout << "u " << trapezoids[updIdx][3] << '\n';
            sgt.upd(trapezoids[updIdx][3], dp[updIdx]);
        }

        std::pair<int, int> maxSet = defaultValue();

        if (t[2] - 1 >= 0) {
            // std::cout << "q " << t[2] - 1 << '\n';
            maxSet = sgt.query(0, t[2]-1);
        }

        maxSet.first++;
        // std::cout << i << ' ' << maxSet.first << ' ' << maxSet.second << '\n';

        dp[i] = maxSet;

        if (ans.first == maxSet.first) {
            ans.second = (ans.second + maxSet.second) % g_k_Mod;
        } else if (maxSet.first > ans.first) {
            ans = maxSet;
        }

        pq.push(std::make_pair(-t[1], i));
    }

    std::cout << ans.first << ' ' << ans.second;
}
