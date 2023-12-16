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
    constexpr __uint128_t lwall = (__uint128_t)1<<(gridsize-1);
    for (int i = 0; i < gridsize; i++) {
        do {
            tomove = rocks[i] & ~(((blocks[i] | rocks[i]) >> 1) | lwall); // rocks to the right of an empty space
            rocks[i] = (rocks[i] & ~tomove) | (tomove << 1);
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
    for (int i = 0; i < gridsize; i++) {
        do {
            tomove = rocks[i] & ~(((blocks[i] | rocks[i]) << 1) | 1); // rocks to the left of an empty space
            rocks[i] = (rocks[i] & ~tomove) | (tomove >> 1);
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


pair<__uint128_t,int> cycle(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
    fallNorth(rocks,blocks);
    fallWest(rocks,blocks);
    fallSouth(rocks,blocks);
    return fallEast(rocks,blocks);
}

void pprint(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
    for (int i = 0; i < gridsize; i++) {
        for (int j = 0; j < gridsize; j++) {
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
    unordered_map<__uint128_t,int> hist;
    unordered_map<int,int> weights;
    int iter = 0;
    __uint128_t lasthash = 0;
    while (hist.find(lasthash) == hist.end()) {
        auto [h,w] = cycle(rocks,blocks);
        hist[lasthash] = iter;
        lasthash = h;
        weights[iter] = w;
        iter++;
    }
    int cycstart = hist[lasthash];
    int cyclen = iter - cycstart;
    int leftover = (1000000000 - cycstart)%cyclen;
    
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
        for (int i = line.size()-1; i >= 0; i--) {
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
    cout << "Average Execution Time (" << ntrials << " trials): " << duration.count()/ntrials << " µs" << endl;
    cout << "Average Time in E/W: " << ineastwest/ntrials << " µs" << endl;
    cout << "Average Time in N/S: " << innorthsouth/ntrials << " µs" << endl;
    return 0;
}