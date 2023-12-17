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

    vector<State> opts() const {
        vector<State> o;
        for (int d : {(this->dir+3)%4, this->dir, (this->dir+1)%4}) {
            auto [di,dj] = dirs[d];
            if (i + di >= 0 && i + di < maxi && j + dj >= 0 && j + dj < maxj) {
                if ((d == this->dir && straight < 10) || (d != this->dir && straight >= 4)) {
                    o.emplace_back(i+di, j+dj, (d == this->dir) ? this->straight+1 : 1, d);
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

struct Hueristic {
    const unordered_map<State,int,StateHash> &dists;

    Hueristic(const unordered_map<State,int,StateHash> &d) : dists(d) {}

    bool operator()(const State &a, const State &b) const {
        return dists.at(a) + (maxi - a.i) + (maxj - a.j) > dists.at(b) + (maxi - b.i) + (maxj - b.j);
    }
};

int shortestPath(State start, vector<vector<int>> &grid) {
    unordered_map<State,int,StateHash> dists;
    Hueristic h(dists);
    priority_queue<State, vector<State>, Hueristic> open(h);

    State s1(start.i, start.j, start.straight, 0);
    State s2(start.i, start.j, start.straight, 1);
    dists[s1] = 0;
    dists[s2] = 0;
    open.push(s1);
    open.push(s2);

    while (!open.empty()) {
        auto current = open.top();

        if (current.i == maxi-1 && current.j == maxj-1 && current.straight >= 4) {
            int d = dists[current];
            return d;
        }
        open.pop();

        for (auto opt : current.opts()) {
            int tdist = dists[current] + grid[opt.i][opt.j];
            if (dists.find(opt) == dists.end() || tdist < dists.at(opt)) {
                dists[opt] = tdist;
                open.push(opt);
            }
        }
    }
    return -1;
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
    cout << shortestPath({0,0,0,0},grid) << endl;
}