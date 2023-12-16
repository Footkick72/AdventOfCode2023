#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>

using namespace std;

constexpr int gridsize = 100;

// << is toward siginifance and also physically left
// so the left wall is 100000... (1<<99)

long long ineastwest = 0;
long long innorthsouth = 0;
// for speed, this function (the last in the cycle call) also computes the hash and the weight on the north beams
pair<__uint128_t,int> fallEast(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
    auto start_time = chrono::high_resolution_clock::now();
    __uint128_t hashval = 0;
    int northweight = 0;

    __uint128_t tomove;
    __uint128_t obst;
    for (int i = 0; i < gridsize; i++) {
        obst = ~((blocks[i] << 1) | 1);
        do {
            tomove = rocks[i] & ~(rocks[i] << 1) & obst; // rocks to the left of an empty space
            rocks[i] &= ~tomove;
            rocks[i] |= tomove >> 1;
        } while (tomove != 0);
        hashval += rocks[i] ^ (blocks[i]<<3);
        hashval *= 13;
        northweight += (gridsize-i)*(__builtin_popcountl(rocks[i]) + __builtin_popcountl(rocks[i]>>64));
    }
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    ineastwest += duration.count();
    return make_pair(hashval, northweight);
}

void fallWest(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
    auto start_time = chrono::high_resolution_clock::now();
    __uint128_t tomove;
    __uint128_t obst;
    constexpr __uint128_t lwall = (__uint128_t)1<<(gridsize-1);
    for (int i = 0; i < gridsize; i++) {
        obst = ~((blocks[i] >> 1) | lwall);
        do {
            tomove = rocks[i] & ~(rocks[i] >> 1) & obst; // rocks to the right of an empty space
            rocks[i] &= ~tomove;
            rocks[i] |= tomove << 1;
        } while (tomove != 0);
    }
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    ineastwest += duration.count();
}

void fallNorth(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
    auto start_time = chrono::high_resolution_clock::now();
    bool alldone;
    int starti = 0;
    do {
        alldone = true;
        for (int i = starti; i < (gridsize-1); i++) {
            __uint128_t tomove = rocks[i+1] & (~(rocks[i] | blocks[i])); // rocks in the lower row where I don't have a rock
            if (tomove != 0) {
                rocks[i+1] ^= tomove;
                rocks[i] |= tomove;
                alldone = false;
            } else if (alldone) {
                starti++;
            }
        }
        starti = max(0,starti-1);
    } while(!alldone);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    innorthsouth += duration.count();
}

void fallSouth(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
    auto start_time = chrono::high_resolution_clock::now();
    bool alldone;
    int starti = (gridsize-1);
    do {
        alldone = true;
        for (int i = (gridsize-1); i > 0; i--) {
            __uint128_t tomove = rocks[i-1] & (~(rocks[i] | blocks[i])); // rocks in the higher row where I don't have a rock
            if (tomove != 0) {
                rocks[i-1] ^= tomove;
                rocks[i] |= tomove;
                alldone = false;
            } else if (alldone) {
                starti--;
            }
        }
       starti = min((gridsize-1),starti+1);
    } while(!alldone);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    innorthsouth += duration.count();
}


long long solvetime = 0;
pair<__uint128_t,int> cycle(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
    fallNorth(rocks,blocks);
    fallWest(rocks,blocks);
    fallSouth(rocks,blocks);
    return fallEast(rocks,blocks);
}

void pprint(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
    for (int i = 0; i < gridsize; i++) {
        for (int j = gridsize-1; j >= 0; j--) {
            if (rocks[i] & ((__uint128_t)1<<j)) {
                cout << 'O';
            } else if (blocks[i] & ((__uint128_t)1<<j)) {
                cout << '#';
            } else {
                cout << '.';
            }
        }
        cout << endl;
    }
    cout << endl;
}

int solve(vector<__uint128_t> rocks, vector<__uint128_t> blocks) {
    auto start_time = chrono::high_resolution_clock::now();

    unordered_map<__uint128_t,int> hist;
    vector<int> weights;
    int iter = 0;
    __uint128_t lasthash = 0;
    while (hist.find(lasthash) == hist.end()) {
        auto s = chrono::high_resolution_clock::now();
        auto [h,w] = cycle(rocks,blocks);
        auto e = chrono::high_resolution_clock::now();
        auto d = chrono::duration_cast<std::chrono::microseconds>(e - s);
        solvetime -= d.count();
        hist[lasthash] = iter;
        lasthash = h;
        weights.push_back(w);
        iter++;
    }
    int cycstart = hist[lasthash];
    int cyclen = iter - cycstart;
    int leftover = (1000000000 - cycstart)%cyclen;

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    solvetime += duration.count();
    
    return weights[cycstart+leftover-1];
}

int main() {
    ios_base::sync_with_stdio(false);
    string line;
    vector<__uint128_t> rocks;
    vector<__uint128_t> blocks;
    while (getline(cin, line)) {
        __uint128_t r = 0;
        __uint128_t b = 0;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == 'O') {
                r++;
            } else if (line[i] == '#') {
                b++;
            }
            r <<= 1;
            b <<= 1;
        }
        r >>= 1;
        b >>= 1;
        rocks.push_back(r);
        blocks.push_back(b);
    }

    constexpr int ntrials = 1000;
    auto start_time = chrono::high_resolution_clock::now();
    for (int _ = 0; _ < ntrials-1; _++) {
        solve(rocks,blocks);
    }
    cout << solve(rocks,blocks) << endl;
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    cout << "Average Execution Time (" << ntrials << " trials): " << duration.count()/(double)ntrials << " µs" << endl;
    cout << "Average Time in E/W: " << ineastwest/(double)ntrials << " µs" << endl;
    cout << "Average Time in N/S: " << innorthsouth/(double)ntrials << " µs" << endl;
    cout << "Average Time in Solve: " << solvetime/(double)ntrials << " µs" << endl;
    cout << "Average Time Unaccounted For: " << (duration.count()-solvetime-ineastwest-innorthsouth)/(double)ntrials << " µs" << endl;
    return 0;
}