#include <iostream>
#include <vector>
#include <map>

using namespace std;

void fallNorth(vector<string> &grid) {
    for (int j = 0; j < grid[0].size(); j++) {
        int rollpos = -1;
        for (int i = 0; i < grid.size(); i++) {
            if (grid[i][j] == 'O') {
                rollpos++;
                grid[i][j] = '.';
                grid[rollpos][j] = 'O'; 
            } else if (grid[i][j] == '#') { 
                rollpos = i;
            }
        }
    }
}

void rotate(vector<string> &grid) {
    /*
    not-at-all-overcomplicated grid rotation in place code
    1 2 3    7 4 1
    4 5 6 -> 8 5 2
    7 8 9    9 6 3
    (i,j) -> (j,size()-1-i)

    ##..
    ##..
    ....
    ....

    ##...
    ##...
    ##...
    .....
    .....

    0 < i < ceil(size()/2)
    0 < j < floor(size()/2)
    */
    for (int i = 0; i < (int)ceil(grid.size()/2.0); i++) {
        for (int j = 0; j < grid[0].size()/2; j++) {
            char t = grid[i][j];
            grid[i][j] = grid[grid.size()-1-j][i];
            grid[grid.size()-1-j][i] = grid[grid.size()-1-i][grid.size()-1-j];
            grid[grid.size()-1-i][grid.size()-1-j] = grid[j][grid.size()-1-i];
            grid[j][grid.size()-1-i] = t;
        }
    }
}

void cycle(vector<string> &grid) {
    fallNorth(grid);
    rotate(grid);
    fallNorth(grid);
    rotate(grid);
    fallNorth(grid);
    rotate(grid);
    fallNorth(grid);
    rotate(grid);
}

int ghash(vector<string> &grid) {
    int res = 0;
    for (int i = 0; i < grid.size(); i++) {
        res += hash<string>{}(grid[i])*7;
    }
    return res;
}

int main() {
    string line;
    vector<string> grid;
    while (getline(cin, line)) {
        grid.push_back(line);
    }
    map<int,int> hist;
    int iter = 0;
    while (hist.find(ghash(grid)) == hist.end()) {
        hist[ghash(grid)] = iter;
        cycle(grid);
        iter++;
    }
    int cycstart = hist[ghash(grid)];
    int cyclen = iter - cycstart;
    int leftover = (1000000000 - cycstart)%cyclen;
    for (int _ = 0; _ < leftover; _++) cycle(grid);
    
    int ans = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == 'O') {
                ans += grid.size()-i;
            }
        }
    }
    cout << ans << endl;
    return 0;
}