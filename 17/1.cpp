#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

int gridsize;

pair<int,int> dirs[4] = {
    {0,1},
    {1,0},
    {0,-1},
    {-1,0},
};

struct State {
    int i, j, straight, dir;

    State(int i, int j, int s, int d) : i(i), j(j), straight(s), dir(d) {}

    State(const State &o) {
        this->i = o.i;
        this->j = o.j;
        this->straight = o.straight;
        this->dir = o.dir;
    }

    vector<State> opts() const {
        vector<State> o;
        for (int d : {(this->dir+3)%4, this->dir, (this->dir+1)%4}) {
            auto [di,dj] = dirs[d];
            if (i + di >= 0 && i + di < gridsize && j + dj >= 0 && j + dj < gridsize) {
                if (d != this->dir || straight < 3) {
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
        return dists.at(a) + (gridsize - a.i) + (gridsize - a.j) > dists.at(b) + (gridsize - b.i) + (gridsize - b.j);
    }
};

pair<int,vector<pair<int,int>>> shortestPath(State start, vector<vector<int>> &grid) {
    unordered_map<State,int,StateHash> dists;
    Hueristic h(dists);
    priority_queue<State, vector<State>, Hueristic> open(h);
    unordered_set<State, StateHash> openset;
    unordered_map<State, State, StateHash> path;

    State s1(start.i, start.j, start.straight, 0);
    State s2(start.i, start.j, start.straight, 1);
    dists[s1] = 0;
    dists[s2] = 0;
    open.push(s1);
    open.push(s2);
    openset.insert(s1);
    openset.insert(s2);

    while (!open.empty()) {
        auto current = open.top();

        if (current.i == gridsize-1 && current.j == gridsize-1) {
            int d = dists[current];
            vector<pair<int,int>> p;
            while (path.find(current) != path.end()) {
                p.push_back({current.i,current.j});
                current = path.at(current);
            }
            return {d,p};
        }
        open.pop();
        openset.erase(current);

        for (auto opt : current.opts()) {
            int tdist = dists[current] + grid[opt.i][opt.j];
            if (dists.find(opt) == dists.end() || tdist < dists.at(opt)) {
                dists[opt] = tdist;
                path.insert_or_assign(opt, current);
                if (openset.find(opt) == openset.end()) {
                    open.push(opt);
                    openset.insert(opt);
                }
            }
        }
    }
    return {-1,{}};
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
    gridsize = grid.size();
    auto [d,p] = shortestPath({0,0,0,0},grid);
    cout << d << endl;
}