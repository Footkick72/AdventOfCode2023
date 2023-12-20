#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

pair<vector<__uint128_t>,vector<pair<int,int>>> splitter_branch(pair<int,int> splitter, const vector<string> &grid) {
    vector<__uint128_t> energized(grid.size(),0);
    
    pair<int,int> dirs[2];
    if (grid[splitter.first][splitter.second] == '|') {
        dirs[0] = {-1,0};
        dirs[1] = {1,0};
    } else if (grid[splitter.first][splitter.second] == '-') {
        dirs[0] = {0,-1};
        dirs[1] = {0,1};
    } else assert(false);

    vector<pair<int,int>> child_splitters;
    for (auto dir : dirs) {
        pair<int,int> pos = splitter;
        bool firststep = true;
        while (true) {
            if (pos.first < 0 || pos.second < 0 || pos.first >= grid.size() || pos.second >= grid[0].size()) {
                break;
            }

            if (!firststep && ((grid[pos.first][pos.second] == '|' && dir.second != 0) || (grid[pos.first][pos.second] == '-' && dir.first != 0))) {
                child_splitters.push_back(pos);
                break;
            }
            firststep = false;

            energized[pos.first] |= ((__uint128_t)1 << pos.second);

            if (grid[pos.first][pos.second] == '/') {
                dir = {-dir.second, -dir.first};
            } else if (grid[pos.first][pos.second] == '\\') {
                dir = {dir.second, dir.first};
            }
            pos = {pos.first + dir.first, pos.second + dir.second};
        }
    }
    return {energized,child_splitters};
}

vector<vector<__uint128_t>> get_best_direct_activation(pair<int,int> splitter, const vector<string> &grid) {
    vector<vector<__uint128_t>> energized;

    pair<int,int> dirs[2];
    if (grid[splitter.first][splitter.second] == '|') {
        dirs[0] = {0,-1};
        dirs[1] = {0,1};
    } else if (grid[splitter.first][splitter.second] == '-') {
        dirs[0] = {-1,0};
        dirs[1] = {1,0};
    } else assert(false);

    for (int i = 0; i < 2; i++) {
        auto dir = dirs[i];
        vector<__uint128_t> energ(grid.size(),0);
        pair<int,int> pos = splitter;
        bool valid = true;
        bool firststep = true;
        while (true) {
            if (pos.first < 0 || pos.second < 0 || pos.first >= grid.size() || pos.second >= grid[0].size()) {
                break;
            }
            
            if (!firststep && ((grid[pos.first][pos.second] == '|' && dir.second != 0) || (grid[pos.first][pos.second] == '-' && dir.first != 0))) {
                valid = false;
                break;
            }
            firststep = false;

            energ[pos.first] |= ((__uint128_t)1 << pos.second);

            if (grid[pos.first][pos.second] == '/') {
                dir = {-dir.second, -dir.first};
            } else if (grid[pos.first][pos.second] == '\\') {
                dir = {dir.second, dir.first};
            }
            pos = {pos.first + dir.first, pos.second + dir.second};
        }
        if (valid) {
            energized.emplace_back(std::move(energ));
        }
    }
    return energized;
}

struct PairHash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        
        return hash1 ^ hash2;
    }
};

vector<__uint128_t> get_energized(
    pair<int,int> splitter, unordered_set<pair<int,int>,PairHash> &seen,
    const unordered_map<pair<int,int>,pair<vector<__uint128_t>,vector<pair<int,int>>>,PairHash> &splitters,
    const vector<string> &grid) {
    
    auto energized = splitters.at(splitter).first;
    
    // cout << "searching " << splitter.first << ',' << splitter.second << endl;
    // for (int i = 0; i < grid.size(); i++) {
    //     for (int j = 0; j < grid[0].size(); j++) {
    //         if ((energized[i]) & ((__uint128_t)1 << j)) {
    //             cout << '#';
    //         } else {
    //             cout << '.';
    //         }
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    
    if (auto p = seen.find(splitter); p != seen.end()) {
        return energized;
    }
    seen.insert(splitter);

    for (auto subsplit : splitters.at(splitter).second) {
        auto e = get_energized(subsplit,seen,splitters,grid);
        for (int i = 0; i < grid.size(); i++) {
            energized[i] |= e[i];
        }
    }


    return energized;
}

int solve(const vector<string> &grid) {
    unordered_map<pair<int,int>,pair<vector<__uint128_t>,vector<pair<int,int>>>,PairHash> splitters;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (splitters.find({i,j}) != splitters.end()) continue;
            if (grid[i][j] == '|' || grid[i][j] == '-') {
                splitters[{i,j}] = splitter_branch({i,j}, grid);
            }
        }
    }
    unordered_set<pair<int,int>,PairHash> tosearch;
    for (auto s : splitters) {
        tosearch.insert(s.first);
    }

    int best = 0;
    while (!tosearch.empty()) {
        auto s = *tosearch.begin();
        tosearch.erase(tosearch.begin());
        
        unordered_set<pair<int,int>,PairHash> seen;
        auto eset = get_energized(s,seen,splitters,grid);
        for (auto d : get_best_direct_activation(s,grid)) {
            int a = 0;
            for (int i = 0; i < grid.size(); i++) {
                auto r = eset[i] | d[i];
                a += __builtin_popcountl(r) + __builtin_popcountl(r>>64);
            }
            best = max(best,a);
        }
    }
    return best;
}

int main() {
    string line;
    vector<string> grid;
    while (getline(cin, line)) {
        grid.push_back(line);
    }
    cout << solve(grid) << endl;
}