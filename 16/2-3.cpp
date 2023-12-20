#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <tuple>

using namespace std;

void strongconnect(tuple<int,int,int,int> v, const vector<string> &grid, map<tuple<int,int,int,int>, int> &indecies, int &nextindex, vector<int> &lowlink, vector<bool> &onstack, vector<tuple<int,int,int,int>> &stack, vector<vector<tuple<int,int,int,int>>> &ccs, map<tuple<int,int,int,int>,vector<tuple<int,int,int,int>>&> &ccmemberships) {
    indecies[v] = nextindex;
    lowlink[nextindex] = nextindex;
    nextindex++;
    stack.push_back(v);
    onstack[indecies[v]] = true;

    vector<tuple<int,int,int,int>> neighbors;
    if (grid[get<0>(v)][get<1>(v)] == '.') {
        neighbors.emplace_back(get<0>(v)+get<2>(v),get<1>(v)+get<3>(v),get<2>(v),get<3>(v));
    } else if (grid[get<0>(v)][get<1>(v)] == '/') {
        neighbors.emplace_back(get<0>(v)-get<3>(v),get<1>(v)-get<2>(v),-get<3>(v),-get<2>(v));
    } else if (grid[get<0>(v)][get<1>(v)] == '\\') {
        neighbors.emplace_back(get<0>(v)+get<3>(v),get<1>(v)+get<2>(v),get<3>(v),get<2>(v));
    } else if (grid[get<0>(v)][get<1>(v)] == '|' && get<2>(v) == 0) {
        neighbors.emplace_back(get<0>(v)-1,get<1>(v),-1,0);
        neighbors.emplace_back(get<0>(v)+1,get<1>(v),1,0);
    } else if (grid[get<0>(v)][get<1>(v)] == '-' && get<3>(v) == 0) {
        neighbors.emplace_back(get<0>(v),get<1>(v)-1,0,-1);
        neighbors.emplace_back(get<0>(v),get<1>(v)+1,0,1);
    }
    for (auto w : neighbors) {
        if (indecies.find(w) == indecies.end()) {
            strongconnect(w,grid,indecies,nextindex,lowlink,onstack,stack,ccs,ccmemberships);
            lowlink[indecies[v]] = min(lowlink[indecies[v]],lowlink[indecies[w]]);
        } else if (onstack[indecies[w]]) {
            lowlink[indecies[v]] = min(lowlink[indecies[v]],indecies[w]);
        }
    }

    if (lowlink[indecies[v]] == indecies[v]) {
        ccs.emplace_back();
        tuple<int,int,int,int> w;
        do {
            w = stack.back();
            stack.pop_back();
            onstack[indecies[w]] = false;
            ccs.back().push_back(w);
            ccmemberships.insert_or_assign(w, ccs.back());
        } while (w != v);
    }
}

int solve(const vector<string> &grid) {

    map<tuple<int,int,int,int>, int> indecies;
    int nextindex = 0;
    vector<int> lowlink(grid.size() * grid[0].size());
    vector<bool> onstack(grid.size() * grid[0].size());
    vector<tuple<int,int,int,int>> stack;
    vector<vector<tuple<int,int,int,int>>> ccs;
    map<tuple<int,int,int,int>,vector<tuple<int,int,int,int>>&> ccmemberships;

    vector<pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            for (auto [di,dj] : dirs) {
                tuple<int,int,int,int> v(i,j,di,dj);
                if (indecies.find(v) == indecies.end()) {
                    strongconnect(v, grid, indecies, nextindex, lowlink, onstack, stack, ccs, ccmemberships);
                }
            }
        }
    }


    for (auto cc : ccs) {
        if (cc.size() > 1) {
            cout << cc.size() << endl;
            cout << endl;
        }
    }

    return 0;
}

int main() {
    string line;
    vector<string> grid;
    while (getline(cin, line)) {
        grid.push_back(line);
    }
    cout << solve(grid) << endl;
}