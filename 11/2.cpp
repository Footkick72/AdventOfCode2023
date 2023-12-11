#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int bsearch(int target, vector<int> &vec) {
    int l = 0;
    int r = vec.size();
    while (l != r) {
        auto midp = (l + r)/2;
        if (vec[midp] > target) {
            r = midp;
        } else if (vec[midp] < target) {
            l = midp+1;
        } else {
            // found it
            return midp;
        }
    }
    return l;
}

int main() {
    string line;
    vector<pair<int,int>> galaxies;
    set<int> filledisset;
    set<int> filledjsset;
    int i = 0, j = 0;
    while (getline(cin,line)) {
        j = 0;
        for (char c : line) {
            if (c == '#') {
                galaxies.emplace_back(i,j);
                filledisset.emplace(i);
                filledjsset.emplace(j);
            }
            j++;
        }
        i++;
    }
    vector<int> filledis(filledisset.begin(), filledisset.end());
    vector<int> filledjs(filledjsset.begin(), filledjsset.end());
    sort(filledis.begin(), filledis.end());
    sort(filledjs.begin(), filledjs.end());
    long long ans = 0;
    for (auto g1 = galaxies.begin(); g1 != galaxies.end(); g1++) {
        for (auto g2 = g1+1; g2 != galaxies.end(); g2++) {
            long long di = abs(g1->first - g2->first) + (1000000-1)*(abs(g1->first - g2->first) - abs(bsearch(g1->first, filledis) - bsearch(g2->first, filledis)));
            long long dj = abs(g1->second - g2->second) + (1000000-1)*(abs(g1->second - g2->second) - abs(bsearch(g1->second, filledjs) - bsearch(g2->second, filledjs)));
            ans += di + dj;
        }
    }
    cout << ans << endl;
    return 0;
}