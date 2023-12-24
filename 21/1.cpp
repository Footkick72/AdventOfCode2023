#include <iostream>
#include <vector>
#include <deque>
#include <tuple>
#include <set>

using namespace std;

int main() {
    int move_budget = 64;

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

    // for (int move_budget = 7; move_budget <= 47; move_budget+=2) {
    int ans = 0;
    deque<tuple<int,int,int>> frontier = {{si,sj,0}};
    set<pair<int,int>> seen;
    set<pair<int,int>> counted;
    while (!frontier.empty()) {
        auto [i,j,d] = frontier.front();
        frontier.pop_front();

        if (i < 0 || j < 0 || i >= grid.size() || j >= grid[0].size()) continue;
        if (grid[i][j] == '#') continue;
        if (seen.find({i,j}) != seen.end()) continue;
        if (d > move_budget) continue;
        seen.insert({i,j});

        if (d % 2 == 0) {ans++; counted.insert({i,j});}
        
        frontier.push_back({i+1,j,d+1});
        frontier.push_back({i-1,j,d+1});
        frontier.push_back({i,j+1,d+1});
        frontier.push_back({i,j-1,d+1});
    }

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

    cout << ans << endl;
    // }
}