#include <iostream>
#include <vector>
#include <map>
#include <chrono>

using namespace std;

constexpr int gridsize = 100;

// << is toward siginifance and also physically left
// so the left wall is 100000... (1<<99)

void fallWest(vector<__int128_t> &rocks, vector<__int128_t> &blocks) {
    __int128_t tomove;
    constexpr __int128_t lwall = (__int128_t)1<<(gridsize-1);
    for (int i = 0; i < gridsize; i++) {
        do {
            tomove = rocks[i] & ~(((blocks[i] | rocks[i]) >> 1) | lwall); // rocks to the right of an empty space
            rocks[i] = (rocks[i] & ~tomove) | (tomove << 1);
        } while (tomove != 0);
    }
}

void fallEast(vector<__int128_t> &rocks, vector<__int128_t> &blocks) {
    __int128_t tomove;
    for (int i = 0; i < gridsize; i++) {
        do {
            tomove = rocks[i] & ~(((blocks[i] | rocks[i]) << 1) | 1); // rocks to the left of an empty space
            rocks[i] = (rocks[i] & ~tomove) | (tomove >> 1);
        } while (tomove != 0);
    }
}

void fallNorth(vector<__int128_t> &rocks, vector<__int128_t> &blocks) {
    bool alldone;
    do {
        alldone = true;
        for (int i = 0; i < (gridsize-1); i++) {
            __int128_t tomove = rocks[i+1] & (~(rocks[i] | blocks[i])); // rocks in the lower row where I don't have a rock
            if (tomove != 0) {
                rocks[i+1] ^= tomove;
                rocks[i] |= tomove;
                alldone = false;
            }
        }
    } while(!alldone);
}

void fallSouth(vector<__int128_t> &rocks, vector<__int128_t> &blocks) {
    bool alldone;
    do {
        alldone = true;
        for (int i = (gridsize-1); i > 0; i--) {
            __int128_t tomove = rocks[i-1] & (~(rocks[i] | blocks[i])); // rocks in the higher row where I don't have a rock
            if (tomove != 0) {
                rocks[i-1] ^= tomove;
                rocks[i] |= tomove;
                alldone = false;
            }
        }
    } while(!alldone);
}


void cycle(vector<__int128_t> &rocks, vector<__int128_t> &blocks) {
    fallNorth(rocks,blocks);
    fallEast(rocks,blocks);
    fallSouth(rocks,blocks);
    fallWest(rocks,blocks);
}

__int128_t ghash(vector<__int128_t> &rocks, vector<__int128_t> &blocks) {
    __int128_t h = 0;
    for (int i = 0; i < rocks.size(); i++) {
        h += rocks[i] ^ (blocks[i]<<3);
        h *= 17;
    }
    return h;
}

void pprint(vector<__int128_t> &rocks, vector<__int128_t> &blocks) {
    for (int i = 0; i < gridsize; i++) {
        for (int j = 0; j < gridsize; j++) {
            if (rocks[i] & ((__int128_t)1<<j)) {
                cout << 'O';
            } else if (blocks[i] & ((__int128_t)1<<j)) {
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
    string line;
    vector<__int128_t> rocks;
    vector<__int128_t> blocks;
    while (getline(cin, line)) {
        __int128_t r = 0;
        __int128_t b = 0;
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

    map<__int128_t,int> hist;
    int iter = 0;
    while (hist.find(ghash(rocks,blocks)) == hist.end()) {
        hist[ghash(rocks,blocks)] = iter;
        cycle(rocks,blocks);
        iter++;
    }
    int cycstart = hist[ghash(rocks,blocks)];
    int cyclen = iter - cycstart;
    int leftover = (1000000000 - cycstart)%cyclen;
    for (int _ = 0; _ < leftover; _++) cycle(rocks,blocks);
    int ans = 0;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if ((rocks[i] & ((__int128_t)1<<j)) > 0) {
                ans += gridsize-i;
            }
        }
    }
    cout << ans << endl;

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    cout << "Execution Time: " << duration.count() << " microseconds" << std::endl;
    return 0;
}