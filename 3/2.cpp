#include <iostream>

using namespace std;

int main() {
    vector<vector<int>> grid;
    string line;
    while (getline(cin, line)) {
        vector<int> l;
        for (auto i = line.begin(); i != line.end(); i++) {
            if (*i == '.') {
                l.push_back(-3);
            } else if (*i == '*') {
                l.push_back(-2);
            } else if (isdigit(*i)) {
                auto j = i;
                int n = 0;
                while (i != line.end() && isdigit(*i)) {
                    n = n * 10 + (*i - '0');
                    i++;
                }
                for (auto k = j; k != i; k++) {
                    l.push_back(n);
                }
                i--;
            } else {
                l.push_back(-1);
            }
        }
        grid.push_back(l);
    }
    int ans = 0;
    for (int i = 0; i < grid.size(); i++) {
        auto row = grid[i];
        for (int j = 0; j < row.size(); j++) {
            if (row[j] == -2) {
                vector<int> parts;
                for (int dx = -1; dx <= 1; dx++) {
                    int last;
                    for (int dy = -1; dy <= 1; dy++) {
                        if (i + dx >= 0 && i + dx < grid.size() && j + dy >= 0 && j + dy < row.size()) {
                            int x = grid[i+dx][j+dy];
                            if (x >= 0 && x != last) {
                                parts.push_back(x);
                            }
                            last = x;
                        }
                    }
                }
                if (parts.size() == 2) {
                    ans += parts[0] * parts[1];
                }
            }
        }
    }
    cout << ans << endl;
    return 0;
}