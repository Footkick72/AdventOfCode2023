#include <iostream>
#include <set>

using namespace std;

void energize(vector<string> &grid, int i, int j, int di, int dj, set<pair<pair<int,int>,pair<int,int>>> &seen, set<pair<int,int>> &energized, set<pair<int,int>> &exits) {
    if (i<0||i>=grid.size()||j<0||j>=grid[0].size()) {
        exits.emplace(i,j);
        return;
    };
    if (seen.find({{i,j},{di,dj}}) != seen.end()) return;
    seen.insert({{i,j},{di,dj}});
    energized.insert({i,j});
    if (grid[i][j]=='.') energize(grid, i+di, j+dj, di, dj, seen, energized, exits);
    if (grid[i][j]=='/') {
        if (di==0) energize(grid, (dj > 0) ? i-1 : i+1, j, (dj > 0) ? -1 : 1, 0, seen, energized, exits);
        if (dj==0) energize(grid, i, (di < 0) ? j+1 : j-1, 0, (di < 0) ? 1 : -1, seen, energized, exits);
    }
    if (grid[i][j]=='\\') {
        if (di==0) energize(grid, (dj < 0) ? i-1 : i+1, j, (dj < 0) ? -1 : 1, 0, seen, energized, exits);
        if (dj==0) energize(grid, i, (di > 0) ? j+1 : j-1, 0, (di > 0) ? 1 : -1, seen, energized, exits);
    }
    if (grid[i][j]=='|') {
        if (di==0) {
            energize(grid, i+1, j, 1, 0, seen, energized, exits);
            energize(grid, i-1, j, -1, 0, seen, energized, exits);
        } else energize(grid, i+di, j+dj, di, dj, seen, energized, exits);
    }
    if (grid[i][j]=='-') {
        if (dj==0) {
            energize(grid, i, j+1, 0, 1, seen, energized, exits);
            energize(grid, i, j-1, 0, -1, seen, energized, exits);
        } else energize(grid, i+di, j+dj, di, dj, seen, energized, exits);
    }
}


int main() {
    string line;
    vector<string> grid;
    while (getline(cin, line)) {
        grid.push_back(line);
    }
    int best = 0;
    set<pair<int,int>> exits;
    for (int i = 0; i < grid.size(); i++) {
        set<pair<pair<int,int>,pair<int,int>>> seen;
        set<pair<int,int>> energized;
        if (exits.find({i,-1}) == exits.end()) {
            energize(grid,i,0,0,1,seen,energized,exits);
            best = max(best, (int)energized.size());
        }
        
        seen.clear();
        energized.clear();

        if (exits.find({i,grid[0].size()}) == exits.end()) {
            energize(grid,i,grid[0].size()-1,0,-1,seen,energized,exits);
            best = max(best, (int)energized.size());
        }
    }
    for (int j = 0; j < grid[0].size(); j++) {
        set<pair<pair<int,int>,pair<int,int>>> seen;
        set<pair<int,int>> energized;

        if (exits.find({-1,j}) == exits.end()) {
            energize(grid,0,j,1,0,seen,energized,exits);
            best = max(best, (int)energized.size());
        }
        
        seen.clear();
        energized.clear();

        if (exits.find({grid.size(),j}) == exits.end()) {
            energize(grid,grid.size()-1,j,-1,0,seen,energized,exits);
            best = max(best, (int)energized.size());
        }
    }
    std::cout << best << endl;
}