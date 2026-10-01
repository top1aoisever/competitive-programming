#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

typedef long long i64;
typedef unsigned long long u64;

typedef int i32;
typedef unsigned int u32;

void file_IO(const char* inputFile, const char* outputFile) {
    freopen(inputFile, "r", stdin);
    freopen(outputFile, "w", stdout);
}

// O(N log^2(N))

std::vector<i64> coordinates;

i32 get_compressed_coordinate(const i64 &x) {
    i32 compressedCoordinate = std::lower_bound(coordinates.begin(), coordinates.end(), x) - coordinates.begin();

    if (compressedCoordinate > (i32)coordinates.size()) return -1;
    if (coordinates[compressedCoordinate] != x) return -1;
    return compressedCoordinate;
}

struct Path {
    i64 start;
    i64 end;

    i64 otherAxis;

    i32 idx;

    bool direction;

    Path(const i64 &s, const i64 &e, const i64 &o, const i32 &i, const bool &d): start(s), end(e), otherAxis(o), idx(i), direction(d) {}

    friend bool operator <(const Path &a, const Path &b) {
        if (a.otherAxis != b.otherAxis) return a.otherAxis < b.otherAxis;
        else if (a.start == b.start) return a.end < b.end;
        else return a.start < b.start;
    }

    bool same_dimension_collide_check(const Path &other) {
        if (otherAxis != other.otherAxis) return false;
        return start <= other.start && other.start < end;
    }
};

const bool PATH_VERTICAL = false;
const bool PATH_HORIZONTAL = true;

std::vector<Path> fullPaths;
std::vector<Path> compressedPaths;

i32 n;

enum QueryType {
    Add = 1000,
    Remove,
    Ask
};

struct Query {
    i32 from;
    i32 to;

    i32 yCoor;

    QueryType type;

    Query(i32 f, i32 t, i32 y, QueryType ty): from(f), to(t), yCoor(y), type(ty) {}

    friend bool operator<(const Query &a, const Query &b) {
        if (a.yCoor != b.yCoor) return a.yCoor < b.yCoor;
        if (a.type == b.type) return a.from < b.from;
        return a.type == QueryType::Remove;
    }
};

std::vector<i32> bit;

void bit_upd(i32 x, const i32 &v) {
    for (; x < (i32)bit.size(); x += x & -x) bit[x] += v;
}

// [0, x]
i32 bit_query(i32 x) {
    i32 ret = 0;
    for (;x>0;x-=x&-x) ret += bit[x];
    return ret;
}

i32 bit_query_range(const i32 &start, const i32 &end) {
    i32 ret = bit_query(end) - bit_query(start - 1);
    return ret;
}

bool should_collide(const i32 &pathCount) {
    std::vector<Path> verticalPaths;
    std::vector<Path> horizontalPaths;

    for (i32 i = 0; i < pathCount; i++) {
        if (compressedPaths[i].direction == PATH_VERTICAL) verticalPaths.push_back(compressedPaths[i]);
        else horizontalPaths.push_back(compressedPaths[i]);
    }

    std::sort(verticalPaths.begin(), verticalPaths.end());
    std::sort(horizontalPaths.begin(), horizontalPaths.end());

    for (i32 i = 0; i < (i32)verticalPaths.size() - 1; i++) {
        if (verticalPaths[i].same_dimension_collide_check(verticalPaths[i+1])) return true;
    }
    for (i32 i = 0; i < (i32)horizontalPaths.size() - 1; i++) {
        if (horizontalPaths[i].same_dimension_collide_check(horizontalPaths[i+1])) return true;
    }

    std::fill(bit.begin(), bit.end(), 0);

    std::vector<Query> updates, asks;

    for (const Path &p : verticalPaths) {
        // std::cout << p.start << ' ' << p.end << ' ' << p.otherAxis << '\n';
        updates.push_back(Query(p.otherAxis, p.otherAxis, p.start, QueryType::Add));
        updates.push_back(Query(p.otherAxis, p.otherAxis, p.end, QueryType::Remove));
    }

    // std::cout << '\n';
    for (const Path &p : horizontalPaths) {
        asks.push_back(Query(p.start, p.end, p.otherAxis, QueryType::Ask));
    }
    std::sort(updates.begin(), updates.end());
    std::sort(asks.begin(), asks.end());

    i32 updCount = updates.size(), askCount = asks.size();
    i32 updIdx = 0, askIdx = 0;
    while (1) {
        if (updIdx >= updCount) {
            for (; askIdx < askCount; askIdx++) {
                if (bit_query_range(asks[askIdx].from, asks[askIdx].to)) return true;
            }
            break;
        }

        i64 curY = updates[updIdx].yCoor;
        while (updIdx < updCount && updates[updIdx].yCoor == curY && updates[updIdx].type == QueryType::Remove) {
            bit_upd(updates[updIdx].from, -1);
            updIdx++;
        }
        for (; askIdx < askCount; askIdx++) {
            if (asks[askIdx].yCoor > curY) break;
            if (bit_query_range(asks[askIdx].from, asks[askIdx].to)) return true;
        }
        while (updIdx < updCount && updates[updIdx].yCoor == curY && updates[updIdx].type == QueryType::Add) {
            bit_upd(updates[updIdx].from, 1);
            updIdx++;
        }
        if (updIdx >= updCount && askIdx >= askCount) break;
    }

    return false;
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    std::cin >> n;

    i64 cX = 0, cY = 0, nX = 0, nY = 0;

    coordinates.push_back(0);

    for (i32 i = 0; i < n; i++) {
        char direction;
        i32 delta;
        std::cin >> direction >> delta;

        switch(direction) {
            case 'U': {
                nY = cY + delta;
                fullPaths.push_back(Path(cY, nY, cX, i, PATH_VERTICAL));
                break;
            }

            case 'D': {
                nY = cY - delta;
                fullPaths.push_back(Path(cY, nY, cX, i, PATH_VERTICAL));
                break;
            }

            case 'L': {
                nX = cX - delta;
                fullPaths.push_back(Path(cX, nX, cY, i, PATH_HORIZONTAL));
                break;
            }

            case 'R': {
                nX = cX + delta;
                fullPaths.push_back(Path(cX, nX, cY, i, PATH_HORIZONTAL));
                break;
            }
        }

        coordinates.push_back(nX);
        coordinates.push_back(nY);
        cX = nX;
        cY = nY;
    }

    std::sort(coordinates.begin(), coordinates.end());
    coordinates.erase(std::unique(coordinates.begin(), coordinates.end()), coordinates.end());

    for (const Path &p : fullPaths) {
        Path cp = p;
        cp.start = get_compressed_coordinate(p.start) + 1;
        cp.end = get_compressed_coordinate(p.end) + 1;
        if (cp.start > cp.end) std::swap(cp.start, cp.end);
        cp.otherAxis = get_compressed_coordinate(p.otherAxis) + 1;
        compressedPaths.push_back(cp);
    }

    bit = std::vector<i32>(coordinates.size() + 1, 0);
    //
    i32 l = 1, r = n + 1;

    i32 collideAt = n+1;

    while (l < r) {
        i32 m = (l + r) / 2;

        if (should_collide(m)) {
            collideAt = m;
            r = m;
        } else l = m + 1;
    }

    // std::cout << collideAt << '\n';
    // return 0;

    if (collideAt == n + 1) {
        i64 fullDist = 0;
        for (i32 i = 0; i < n; i++) {
            fullDist += std::abs(fullPaths[i].end - fullPaths[i].start);
        }
        std::cout << fullDist;
        return 0;
    }

    i64 distanceBeforeLast = 0;
    i64 lastDistance = 0;
    std::vector<i64> distanceBeforeStop(collideAt-1, -1);

    for (i32 i = 0; i < collideAt - 1; i++) {
        distanceBeforeLast += std::abs(fullPaths[i].end - fullPaths[i].start);
    }

    Path &lastPath = fullPaths[collideAt - 1];
    for (i32 i = 0; i < collideAt - 1; i++) {
        Path &curPath = fullPaths[i];
        if (curPath.direction == lastPath.direction) {
            if (curPath.otherAxis != lastPath.otherAxis) continue;
            i64 curStart = curPath.start;
            i64 curEnd = curPath.end;
            if (curStart > curEnd) std::swap(curStart, curEnd);
            bool leftOrDown = (lastPath.start > lastPath.end);
            if (leftOrDown) {
                if (lastPath.start <= curStart || lastPath.end >= curEnd) continue;
                if (lastPath.start > curStart && lastPath.start <= curEnd) distanceBeforeStop[i] = 0;
                else distanceBeforeStop[i] = lastPath.start - curEnd;
            } else {
                if (lastPath.start >= curEnd || lastPath.end <= curStart) continue;
                if (lastPath.start >= curStart) distanceBeforeStop[i] = 0;
                else {
                    distanceBeforeStop[i] = curStart - lastPath.start;
                }
            }
        }
        else {
            // how did it work ?
            if (lastPath.otherAxis < std::min(curPath.start, curPath.end) || lastPath.otherAxis > std::max(curPath.end, curPath.start)) continue;
            if (curPath.otherAxis <= std::min(lastPath.start, lastPath.end) || curPath.otherAxis >= std::max(lastPath.end, lastPath.start)) continue;
            distanceBeforeStop[i] = std::llabs(lastPath.start - curPath.otherAxis);
        }
    }
    for (const i64 &dis : distanceBeforeStop) {
        // std::cout << dis << '\n';
        if (dis != -1) {
            if (lastDistance == 0) lastDistance = dis;
            else lastDistance = std::min(lastDistance, dis);
        }
    }

    i64 ans = lastDistance + distanceBeforeLast;

    // std::cout << distanceBeforeLast << ' ' << lastDistance << ' ' << ans << '\n';
    std::cout << ans;
    return 0;

}
