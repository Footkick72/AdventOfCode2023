#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>

using namespace std;

constexpr int gridsize = 100;

// << is toward siginifance and also physically left
// so the left wall is 100000... (1<<99)

// for speed, this function (the last in the cycle call) also computes the hash and the weight on the north beams
pair<__uint128_t,int> fallWest(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
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
    
    return make_pair(hashval, northweight);
}

void fallEast(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
    __uint128_t tomove;
    for (int i = 0; i < gridsize; i++) {
        do {
            tomove = rocks[i] & ~(((blocks[i] | rocks[i]) << 1) | 1); // rocks to the left of an empty space
            rocks[i] = (rocks[i] & ~tomove) | (tomove >> 1);
        } while (tomove != 0);
    }
}

void fallNorth(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
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
}

void fallSouth(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
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
        min((gridsize-1),starti+1);
    } while(!alldone);
}


pair<__uint128_t,int> cycle(vector<__uint128_t> &rocks, vector<__uint128_t> &blocks) {
    fallNorth(rocks,blocks);
    fallEast(rocks,blocks);
    fallSouth(rocks,blocks);
    return fallWest(rocks,blocks);
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
}

int main() {
    auto start_time = chrono::high_resolution_clock::now();
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
    
    cout << weights[cycstart+leftover-1] << endl;

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    cout << "Execution Time: " << duration.count() << " microseconds" << std::endl;
    return 0;
}