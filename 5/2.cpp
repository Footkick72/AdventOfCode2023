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

struct range {
    long start;
    long end;
    
    range(long s, long e) : start(s), end(e) {}
};

int main() {
    string line;
    getline(cin, line);
    istringstream ss(line);
    string word;
    ss >> word;
    vector<range> seeds;
    long s, l;
    while (!ss.eof()) {
        ss >> s;
        ss >> l;
        seeds.emplace_back(s, s+l);
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
    for (const auto &seed : seeds) {
        vector<range> sranges = {seed};
        for (const auto &m : maps) {
            vector<range> newsranges = {};
            while (!sranges.empty()) {
                auto s = sranges.back();
                sranges.pop_back();
                bool transformed = false;
                for (const auto &r : m) {
                    if (r.source <= s.start && s.end <= r.source + r.length) {
                        // fully included
                        newsranges.push_back(range((s.start - r.source) + r.dest, (s.end - r.source) + r.dest));
                        transformed = true;
                        break;
                    } else if (s.end <= r.source || r.source + r.length <= s.end) {
                        // fully excluded
                    } else if (r.source <= s.start && s.end > r.source + r.length) {
                        // overhanging to the right
                        newsranges.push_back(range((s.start - r.source) + r.dest, r.source + r.length));
                        sranges.push_back(range(r.source + r.length, s.end));
                        transformed = true;
                    } else if (s.start < r.source && s.end <= r.source + r.length) {
                        // overhanging to the left
                        newsranges.push_back(range(r.dest, (s.end - r.source) + r.dest));
                        sranges.push_back(range(s.start, r.source));
                        transformed = true;
                    }
                }
                if (!transformed) {
                    // if we never overlapped, identity transform
                    newsranges.push_back(s);
                }
            }
            sranges = newsranges;
        }
        for (auto r : sranges) {
            ans = min(ans, r.start);
        }
    }
    cout << ans << endl;
    return 0;
}