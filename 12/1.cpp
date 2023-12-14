#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

long long solve(map<pair<int,int>, long long> &hash, string &record, int r_i, vector<int> &groups, int g_i) {
    while (r_i < record.size() && record[r_i] == '.') {
        r_i++;
    }
    if (r_i == record.size()) {
        return g_i == groups.size() ? 1 : 0;
    }
    if (g_i == groups.size()) {
        while (r_i < record.size() && record[r_i] != '#') {
            r_i++;
        }
        return r_i == record.size() ? 1 : 0;
    }
    auto or_i = r_i;
    auto og_i = g_i;
    if (hash.find(make_pair(r_i, g_i)) != hash.end()) {
        return hash.find(make_pair(r_i, g_i))->second;
    }
    long long ans = 0;
    int g = 0;
    while (r_i < record.size() && record[r_i] != '.') {
        char c = record[r_i];
        r_i++;
        if (c == '?') {
            if (g == groups[g_i]) {
                ans += solve(hash, record, r_i, groups, g_i+1);
            }
            if (g == 0) {
                ans += solve(hash, record, r_i, groups, g_i);
            }
        }
        g++;
    }
    if (g == groups[g_i]) {
        ans += solve(hash, record, r_i, groups, g_i+1);
    }
    hash[make_pair(or_i, og_i)] = ans;
    return ans;
}

int main() {
    string line;
    long long ans = 0;
    while (getline(cin, line)) {
        istringstream ss(line);
        string springs;
        ss >> springs;
        vector<int> groups;
        string g;
        while (getline(ss, g, ',')) {
            groups.push_back(stoi(g));
        }
        map<pair<int,int>, long long> hash;
        ans += solve(hash, springs, 0, groups, 0);
    }
    cout << ans << endl;
    return 0;
}