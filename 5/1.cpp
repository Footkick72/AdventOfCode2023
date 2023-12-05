#include <iostream>
#include <sstream>
#include <vector>
#include <limits>

using namespace std;

struct rangemap {
    long dest;
    long source;
    long length;

    rangemap(long d, long s, long l) : dest(d), source(s), length(l) {}
};

int main() {
    string line;
    getline(cin, line);
    istringstream ss(line);
    string word;
    ss >> word;
    vector<long> seeds;
    while (ss) {
        ss >> word;
        seeds.push_back(stol(word));
    }
    vector<vector<rangemap>> maps;
    while (getline(cin, line)) {
        if (line.empty()) {
            maps.push_back(vector<rangemap>());
        } else if (isdigit(line.front())) {
            istringstream ss(line);
            ss >> word;
            long d = stol(word);
            ss >> word;
            long s = stol(word);
            ss >> word;
            long l = stol(word);
            rangemap r(d,s,l);
            maps.back().push_back(r);
        }
    }
    long ans = numeric_limits<long>::max();
    for (long s : seeds) {
        for (const auto &m : maps) {
            for (const auto &r : m) {
                if (s >= r.source && s < r.source + r.length) {
                    s = (s - r.source) + r.dest;
                    break;
                }
            }
        }
        ans = min(ans, s);
    }
    cout << ans << endl;
    return 0;
}