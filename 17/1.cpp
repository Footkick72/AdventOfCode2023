#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <chrono>

using namespace std;

int maxi;
int maxj;

pair<int,int> dirs[4] = {
    {0,1},
    {1,0},
    {0,-1},
    {-1,0},
};


struct State {
    int i, j, straight, dir;

    State(int i, int j, int s, int d) : i(i), j(j), straight(s), dir(d) {}

    vector<pair<State,int>> opts(vector<vector<int>> &grid) const {
        vector<pair<State,int>> o;
        for (int d : {(this->dir+3)%4, this->dir, (this->dir+1)%4}) {
            auto [di,dj] = dirs[d];
            if (d != this->dir || straight < 3) {
                if (i + di >= 0 && i + di < maxi && j + dj >= 0 && j + dj < maxj) {
                    o.emplace_back(State{i+di, j+dj, (d == this->dir) ? this->straight+1 : 1 , d}, grid[i+di][j+dj]);
                }
            }
        }
        return o;
    }
};

bool operator==(const State &s1, const State &s2) {
    return s1.i == s2.i && s1.j == s2.j && s1.dir == s2.dir && s1.straight == s2.straight;
}

struct StateHash {
    int operator()(const State &s) const {
        return (s.i * 6199) ^ (s.j * 7841) ^ (s.dir * 5009) ^ (s.straight * 7919);
    }
};

struct Heuristic {
    const vector<vector<array<array<int,11>,4>>> &dists;
    const vector<vector<int>> &heuristic;

    Heuristic(const vector<vector<array<array<int,11>,4>>> &d, const vector<vector<int>> &h) : dists(d), heuristic(h) {}

    bool operator()(const State &a, const State &b) const {
        auto d1 = dists[a.i][a.j][a.dir][a.straight] + heuristic[a.i][a.j];
        auto d2 = dists[b.i][b.j][b.dir][b.straight] + heuristic[b.i][b.j];
        if (d1 != d2) return d1 < d2;
        return StateHash{}(a) < StateHash{}(b);
    }
};

int shortestPath(State start, vector<vector<int>> &grid, vector<vector<int>> &heuristic) {
    vector<vector<array<array<int,11>,4>>> dists;
    for (int i = 0; i < maxi; i++) {
        dists.emplace_back();
        for (int j = 0; j < maxj; j++) {
            dists.back().emplace_back();
            for (int d = 0; d < 4; d++) {
                for (int s = 0; s < 11; s++) {
                    dists[i][j][d][s] = INT_MAX;
                }
            }
        }
    }
    Heuristic h(dists, heuristic);
    set<State, Heuristic> open(h);

    State s1(start.i, start.j, start.straight, 0);
    State s2(start.i, start.j, start.straight, 1);
    dists[s1.i][s1.j][s1.dir][s1.straight] = 0;
    dists[s2.i][s2.j][s2.dir][s2.straight] = 0;
    open.insert(s1);
    open.insert(s2);

    while (!open.empty()) {
        auto current = *open.begin();

        if (current.i == maxi-1 && current.j == maxj-1) {
            return dists[current.i][current.j][current.dir][current.straight];
        }
        open.erase(open.begin());

        for (auto opt : current.opts(grid)) {
            int tdist = dists[current.i][current.j][current.dir][current.straight] + opt.second;
            if (tdist < dists[opt.first.i][opt.first.j][opt.first.dir][opt.first.straight]) {
                if (auto p = open.find(opt.first); p != open.end()) open.erase(p);
                dists[opt.first.i][opt.first.j][opt.first.dir][opt.first.straight] = tdist;
                open.insert(opt.first);
            }
        }
    }
    return -1;
}

vector<vector<int>> revDijkstra(vector<vector<int>> &grid) {
    vector<vector<int>> dists;
    for (int i = 0; i < maxi; i++) {
        dists.emplace_back();
        for (int j = 0; j < maxj; j++) {
            dists.back().emplace_back(INT_MAX);
        }
    }
    auto g = [&dists](const pair<int,int> &p1, const pair<int,int> &p2) {
        int d1 = dists[p1.first][p1.second];
        int d2 = dists[p2.first][p2.second];
        if (d1 != d2) return d1 < d2;
        return p1 < p2;
    };
    set<pair<int,int>,decltype(g)> open(g);

    open.insert({maxi-1, maxj-1});
    dists[maxi-1][maxj-1] = 0;

    while (!open.empty()) {
        auto current = *open.begin();
        open.erase(open.begin());

        for (auto [di,dj] : array<array<int,2>,4>{{{0,1},{0,-1},{1,0},{-1,0}}}) {
            if (current.first + di >= 0 && current.first + di < maxi && current.second + dj >= 0 && current.second + dj < maxj) {
                int ni = current.first+di;
                int nj = current.second+dj;
                int tdist = dists[current.first][current.second] + grid[ni][nj];
                if (tdist < dists[ni][nj]) {
                    if (auto p = open.find({ni,nj}); p != open.end()) open.erase(p);
                    dists[ni][nj] = tdist;
                    open.insert({ni,nj});
                }
            }
        }
    }
    return dists;
}

int main() {
    string line;
    vector<vector<int>> grid;
    while (getline(cin,line)) {
        grid.emplace_back();
        for (auto c : line) {
            grid.back().push_back(c-'0');
        }
    }
    maxi = grid.size();
    maxj = grid[0].size();

    auto start_time = chrono::high_resolution_clock::now();
    auto heuristic = revDijkstra(grid);
    start_time = chrono::high_resolution_clock::now();
    cout << shortestPath({0,0,0,0},grid,heuristic) << endl;
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    cout << duration.count()/1000.0 << " ms Total" << endl;
}