#include <iostream>
#include <vector>

using namespace std;

int horizontalgrid(vector<string> &grid) {
    // horizontal line
    for (auto reflecti = 1; reflecti < grid.size(); reflecti++) {
        // reflecti = first line below ><
        bool works = true;
        for (int i = reflecti; i < grid.size(); i++) {
            if (2*reflecti-i-1 < 0) break;
            if (grid[i] != grid[2*reflecti-i-1]) {
                // cout << i << "  " << grid[i] << "" << endl;
                // cout << 2*reflecti-i-1 << "  " << grid[2*reflecti-i-1] << " fails" << endl;
                // cout << endl;
                works = false;
                break;
            }
        }
        if (works) {
            return reflecti;
        }
    }
    return 0;
}

int verticalgrid(vector<string> &grid) {
    // rotate the grid (yes I'm being stupid I don't care :D)
    vector<string> rotgrid;
    for (int _ = 0; _ < grid[0].size(); _++) {
        rotgrid.emplace_back();
    }
    for (auto row : grid) {
        for (int i = 0; i < row.size(); i++) {
            rotgrid[i].push_back(row[i]);
        }
    }
    return horizontalgrid(rotgrid);
    
}

int main() {
    string line;
    vector<string> grid;
    int ans = 0;
    while (getline(cin, line)) {
        if (!line.empty()) {
            grid.push_back(line);
        } else {
            ans += 100 * horizontalgrid(grid) + verticalgrid(grid);
            grid.clear();
        }
    }
    ans += 100 * horizontalgrid(grid) + verticalgrid(grid);
    cout << ans << endl;
    return 0;
}