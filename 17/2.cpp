#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <unordered_set>
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
            if (d != this->dir && straight >= 4) {
                if (i + 4*di >= 0 && i + 4*di < maxi && j + 4*dj >= 0 && j + 4*dj < maxj) {
                    int c = 0;
                    for (int k = 1; k <= 4; k++) c += grid[i+k*di][j+k*dj];
                    o.emplace_back(State{i+4*di, j+4*dj, 4, d}, c);
                }
            }
            if (d == this->dir && straight < 10) {
                if (i + di >= 0 && i + di < maxi && j + dj >= 0 && j + dj < maxj) {
                    o.emplace_back(State{i+di, j+dj, this->straight+1, d}, grid[i+di][j+dj]);
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
    const unordered_map<State,int,StateHash> &dists;
    const vector<vector<int>> &heuristic;

    Heuristic(const unordered_map<State,int,StateHash> &d, const vector<vector<int>> &h) : dists(d), heuristic(h) {}

    bool operator()(const State &a, const State &b) const {
        // return dists.at(a) + (maxi - a.i) + (maxj - a.j) > dists.at(b) + (maxi - b.i) + (maxj - b.j);
        return dists.at(a) + heuristic[a.i][a.j] > dists.at(b) + heuristic[b.i][b.j];
    }
};

int shortestPath(State start, vector<vector<int>> &grid, vector<vector<int>> &heuristic) {
    unordered_map<State,int,StateHash> dists;
    Heuristic h(dists, heuristic);
    priority_queue<State, vector<State>, Heuristic> open(h);

    State s1(start.i, start.j, start.straight, 0);
    State s2(start.i, start.j, start.straight, 1);
    dists[s1] = 0;
    dists[s2] = 0;
    open.push(s1);
    open.push(s2);

    while (!open.empty()) {
        auto current = open.top();

        if (current.i == maxi-1 && current.j == maxj-1 && current.straight >= 4) {
            return dists[current];
        }
        open.pop();

        for (auto opt : current.opts(grid)) {
            int tdist = dists[current] + opt.second;
            if (dists.find(opt.first) == dists.end() || tdist < dists.at(opt.first)) {
                dists[opt.first] = tdist;
                open.push(opt.first);
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
    auto g = [](const pair<int,int> &p1, const pair<int,int> &p2) {
        return p1 > p2;
    };
    priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(g)> open(g);

    open.push({maxi-1, maxj-1});
    dists[maxi-1][maxj-1] = 0;

    while (!open.empty()) {
        auto current = open.top();
        open.pop();

        for (auto [di,dj] : array<array<int,2>,4>{{{0,1},{0,-1},{1,0},{-1,0}}}) {
            if (current.first + di >= 0 && current.first + di < maxi && current.second + dj >= 0 && current.second + dj < maxj) {
                int ni = current.first+di;
                int nj = current.second+dj;
                int tdist = dists[current.first][current.second] + grid[ni][nj];
                if (tdist < dists[ni][nj]) {
                    dists[ni][nj] = tdist;
                    open.push({ni,nj});
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
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    int djikstratime = duration.count();
    start_time = chrono::high_resolution_clock::now();
    cout << shortestPath({0,0,0,0},grid,heuristic) << endl;
    end_time = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    cout << djikstratime/1000.0 << " ms Djikstra Precompute" << endl;
    cout << duration.count()/1000.0 << " ms A*" << endl;
}