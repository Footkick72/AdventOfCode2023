#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

long long solve(map<pair<string,vector<int>>, long long> &hash, string record, vector<int> groups) {
    while (!record.empty() && record.back() == '.') {
        record.pop_back();
    }
    if (record.empty()) {
        return groups.empty() ? 1 : 0;
    }
    if (groups.empty()) {
        while (!record.empty() && record.back() != '#') {
            record.pop_back();
        }        
        return record.empty() ? 1 : 0;
    }
    auto orecord = record;
    auto ogroups = groups;
    if (hash.find(make_pair(record, groups)) != hash.end()) {
        return hash.find(make_pair(record, groups))->second;
    }
    long long ans = 0;
    int g = 0;
    while (!record.empty() && record.back() != '.') {
        char c = record.back();
        record.pop_back();
        if (c == '?') {
            if (g == groups.back()) {
                vector<int> newgroups = groups;
                newgroups.pop_back();
                ans += solve(hash, record, newgroups);
            }
            if (g == 0) {
                ans += solve(hash, record, groups);
            }
        }
        g++;
    }
    if (g == groups.back()) {
        vector<int> newgroups = groups;
        newgroups.pop_back();
        ans += solve(hash, record, newgroups);
    }
    hash[make_pair(orecord, ogroups)] = ans;
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
        vector<int> newgroups;
        for (int _ = 0; _ < 5; _++) {
            for (int g : groups) {
                newgroups.push_back(g);
            }
        }
        springs = springs + "?" + springs + "?" + springs + "?" + springs + "?" + springs;
        map<pair<string,vector<int>>, long long> hash;
        ans += solve(hash, springs, newgroups);
    }
    cout << ans << endl;
    return 0;
}