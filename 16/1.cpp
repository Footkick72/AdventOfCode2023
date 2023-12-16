#include <iostream>
#include <set>

using namespace std;

void energize(vector<string> &grid, int i, int j, int di, int dj, set<pair<pair<int,int>,pair<int,int>>> &seen, set<pair<int,int>> &energized) {
    if (i<0||i>=grid.size()||j<0||j>=grid[0].size()) return;
    if (seen.find({{i,j},{di,dj}}) != seen.end()) return;
    seen.insert({{i,j},{di,dj}});
    energized.insert({i,j});
    if (grid[i][j]=='.') energize(grid, i+di, j+dj, di, dj, seen, energized);
    if (grid[i][j]=='/') {
        if (di==0) energize(grid, (dj > 0) ? i-1 : i+1, j, (dj > 0) ? -1 : 1, 0, seen, energized);
        if (dj==0) energize(grid, i, (di < 0) ? j+1 : j-1, 0, (di < 0) ? 1 : -1, seen, energized);
    }
    if (grid[i][j]=='\\') {
        if (di==0) energize(grid, (dj < 0) ? i-1 : i+1, j, (dj < 0) ? -1 : 1, 0, seen, energized);
        if (dj==0) energize(grid, i, (di > 0) ? j+1 : j-1, 0, (di > 0) ? 1 : -1, seen, energized);
    }
    if (grid[i][j]=='|') {
        if (di==0) {
            energize(grid, i+1, j, 1, 0, seen, energized);
            energize(grid, i-1, j, -1, 0, seen, energized);
        } else energize(grid, i+di, j+dj, di, dj, seen, energized);
    }
    if (grid[i][j]=='-') {
        if (dj==0) {
            energize(grid, i, j+1, 0, 1, seen, energized);
            energize(grid, i, j-1, 0, -1, seen, energized);
        } else energize(grid, i+di, j+dj, di, dj, seen, energized);
    }
}


int main() {
    string line;
    vector<string> grid;
    while (getline(cin, line)) {
        grid.push_back(line);
    }
    set<pair<pair<int,int>,pair<int,int>>> seen;
    set<pair<int,int>> energized;

    energize(grid,0,0,0,1,seen,energized);

    std::cout << energized.size() << endl;
}