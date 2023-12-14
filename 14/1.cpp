#include <iostream>
#include <vector>

using namespace std;

int main() {
    string line;
    vector<string> grid;
    while (getline(cin, line)) {
        grid.push_back(line);
    }
    int ans = 0;
    for (int j = 0; j < grid[0].size(); j++) {
        int rollpos = -1;
        for (int i = 0; i < grid.size(); i++) {
            if (grid[i][j] == 'O') {
                rollpos++;
                ans += grid.size() - rollpos;
            } else if (grid[i][j] == '#') { 
                rollpos = i;
            }
        }
    }
    cout << ans << endl;
    return 0;
}