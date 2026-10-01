#include <iostream>
#include <cstdio>
#include <vector>

typedef long long ll;
typedef unsigned long long ull;

typedef unsigned int uint;

void fileIO(const char* inputFile, const char* outputFile) {
    freopen(inputFile, "r", stdin);
    freopen(outputFile, "w", stdout);
}

void solve() {
}

struct Lazy {
    int value = -1;

    void apply(const Lazy& lz) {
        if (lz.value != -1) value = lz.value;
    }

    Lazy() {}
    Lazy(const int& lval): value(lval) {}
};

struct Node {
    int orig = -1;
    int maxInv = -1;

    void apply(const Lazy& lz) {
        if (lz.value != -1) maxInv = orig + lz.value;
    }

    Node() {}

    Node(const int& o, const int& m): orig(o), maxInv(m) {}
};

Node operator+(const Node& a, const Node& b) {
    Node ret;
    ret.orig = std::max(a.orig, b.orig);
    ret.maxInv = std::max(a.maxInv, b.maxInv);
    return ret;
}

class LazySegtree {
    private:
        int n;
        std::vector<Node> tree;
        std::vector<Lazy> lazy;

        void build(const int& id, const int& l, const int& r, const std::vector<Node>& nodes) {
            if (l == r) {
                tree[id] = nodes[l];
                return;
            }
            int m = (l + r) >> 1;
            build(id<<1, l, m, nodes);
            build(id<<1|1, m+1, r, nodes);
            tree[id] = tree[id<<1] + tree[id<<1|1];
        }

        void apply(const int& id, const int& l, const int& r, const Lazy& lz) {
            tree[id].apply(lz);
            lazy[id].apply(lz);
        }

        void push_down(const int& id, const int& l, const int& r) {
            int m = (l + r) >> 1;
            apply(id<<1, l, m, lazy[id]);
            apply(id<<1|1, m+1, r, lazy[id]);
            lazy[id] = Lazy();
        }

        void upd(const int& id, const int& l, const int& r, const int& ul, const int& ur, const Lazy& lz) {
            if (ul > r || ur < l) return;
            if (ul <= l && r <= ur) {
                apply(id, l, r, lz);
            } else {
                push_down(id, l, r);
                int m = (l + r) >> 1;
                upd(id<<1, l, m, ul, ur, lz);
                upd(id<<1|1, m+1, r, ul, ur, lz);
                tree[id] = tree[id<<1] + tree[id<<1|1];
            }
        }

        Node query(const int& id, const int& l, const int& r, const int& ql, const int& qr) {
            if (ql > r || qr < l) return Node();
            if (ql <= l && r <= qr) return tree[id];

            push_down(id, l, r);
            int m = (l + r) >> 1;
            return query(id<<1, l, m, ql, qr) + query(id<<1|1, m+1, r, ql, qr);
        }

    public:
        LazySegtree(const std::vector<Node>& nodes): n(nodes.size()) {
            tree.assign(4 << std::__lg(n), Node());
            lazy.assign(4 << std::__lg(n), Lazy());
            build(1, 0, n-1, nodes);
        }

        void upd(const int& ul, const int& ur, const Lazy& lz) {
            // std::cout << "Updating... " << lz.value << '\n';
            upd(1, 0, n-1, ul, ur, lz);
        }

        Node query(const int& ql, const int& qr) {
            return query(1, 0, n-1, ql, qr);
        }
};

// TODO: actually make the program run

struct Query {
    int idx = -1;
    int k = -1;
    int end = -1;

    Query(const int& _idx, const int& _k, const int& _end): idx(_idx), k(_k), end(_end) {}
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    int n, m;
    std::cin >> n >> m;

    std::vector<int> books(n);
    std::vector<Node> bookNodes(n);

    for (int i = 0; i < n; i++) {
        std::cin >> books[i];
        bookNodes[i] = Node(books[i], 0);
    }

    std::vector<std::vector<Query>> queries(n);

    for (int i = 0; i < m; i++) {
        int from, to, k;
        std::cin >> from >> to >> k;
        queries[from-1].push_back(Query(i, k, to-1));
    }

    std::vector<bool> ans(m);

    std::vector<int> stack;

    LazySegtree seg(bookNodes);

    for (int i = n-1; i>=0; i--) {
        while (!stack.empty() && books[stack.back()] < books[i]) stack.pop_back();
        int j = stack.empty() ? n : stack.back();
        if (j - i - 1 >= 1) seg.upd(i+1, j-1, Lazy(books[i]));
        stack.push_back(i);
        for (const Query& q : queries[i]) {
            // std::cout << q.idx << ' ' << q.k << ' ' << q.end << '\n';
            int res = seg.query(i, q.end).maxInv;
            // std::cout << "Max inversion " << res << '\n';
            ans[q.idx] = res <= q.k;
        }
    }

    for (const bool& b : ans) std::cout << b << '\n';
}
