#include <iostream>
#include <vector>
#include <deque>
#include <tuple>
#include <set>

using namespace std;

int reachable(vector<string> &grid, int parity, int search_dist, pair<int,int> startpos) {
    deque<tuple<int,int,int>> frontier = {{startpos.first,startpos.second,0}};
    set<pair<int,int>> seen;
    set<pair<int,int>> counted;
    
    int ans = 0;
    while (!frontier.empty()) {
        auto [i,j,d] = frontier.front();
        frontier.pop_front();

        if (i < 0 || j < 0 || i >= grid.size() || j >= grid[0].size()) continue;
        if (grid[i][j] == '#') continue;
        if (d > search_dist) continue;
        if (seen.find({i,j}) != seen.end()) continue;
        seen.insert({i,j});

        if (d % 2 == parity) {ans++; counted.insert({i,j});}
        
        frontier.push_back({i+1,j,d+1});
        frontier.push_back({i-1,j,d+1});
        frontier.push_back({i,j+1,d+1});
        frontier.push_back({i,j-1,d+1});
    }

    // cout << endl;
    // for (int i = 0; i < grid.size(); i++) {
    //     for (int j = 0; j < grid[0].size(); j++) {
    //         if (counted.find({i,j}) != counted.end()) {
    //             cout << 'O';
    //         } else {
    //             cout << grid[i][j];
    //         }
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    return ans;
}

int main() {
    int total_budget = 26501365;
    // int total_budget = 31;

    string line;
    vector<string> grid;
    while(getline(cin,line)) {
        grid.push_back(line);
    }

    int si,sj;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == 'S') {
                si = i;
                sj = j;
            }
        }
    }
    // for (int total_budget = 7; total_budget <= 47; total_budget+=2) {
    int search_dist = ((total_budget-(grid.size()+1)/2) % (grid.size())) - grid.size();
    int manhattan_rad = ((total_budget-(grid.size()+1)/2) / (grid.size())) + 3;

    long ans = 0;
    while (true) {
        // cout << manhattan_rad << endl;
        int diag_search_dist = search_dist - (grid.size()+1)/2 + grid.size();
        long diag_copies = manhattan_rad-2;

        if ((search_dist >= 2*(int)grid.size() && diag_search_dist >= 2*(int)grid.size()) || manhattan_rad == 1) break;
        
        // cout << search_dist << ',' << diag_search_dist << endl;
        // cout << endl;

        for (int ci = 0; ci < 4; ci++) {
            // midpoint of this edge
            pair<int,int> startpos;
            if (ci == 0) startpos = {0, (grid.size()-1)/2};
            else if (ci == 1) startpos = {(grid.size()-1)/2, 0};
            else if (ci == 2) startpos = {grid.size()-1, (grid.size()-1)/2};
            else if (ci == 3) startpos = {(grid.size()-1)/2, grid.size()-1};
            // cout << startpos.first << ',' << startpos.second << endl;
            int reachable_mid = reachable(grid, (total_budget - search_dist + 1)%2, search_dist, startpos);
            ans += reachable_mid;

            // diagonal of this edge
            // cout << (grid.size()-1) * (ci%2) << ',' << (grid.size()-1) * (ci/2) << endl;
            int reachable_diag = reachable(grid, (total_budget - diag_search_dist + 1)%2, diag_search_dist, {(grid.size()-1) * (ci%2), (grid.size()-1) * (ci/2)});
            ans += diag_copies * reachable_diag;
        }

        search_dist += grid.size();
        manhattan_rad -= 1;
        // parity = (parity + grid.size()) % 2;
    }

    int center1 = reachable(grid, total_budget%2 ,INT_MAX, {si,sj}); // actual center square (odd manhattan radii)
    int center2 = reachable(grid, (total_budget+1)%2 ,INT_MAX, {si,sj}); // even manhattan radii

    ans += center1;
    int c = (((grid.size()+1)/2) % 2 == 0) ? 1 : 0;
    for (int m = 2; m <= manhattan_rad; m++) {
        long copies = 4 + (m-2) * 4;
        if (c == 0) {
            ans += copies * center1;
            // cout << copies << " of type 1" << endl;
        } else {
            ans += copies * center2;
            // cout << copies << " of type 2" << endl;
        }
        c = (c+1)%2;
    }

    cout << ans << endl;
    // }
}