#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_set>
#include <numeric>

using namespace std;

struct Node {
    string left;
    string right;

    Node(string l, string r) : left(l), right(r) {};
};

int main() {
    string instructions;
    string line;
    cin >> instructions;
    map<string,Node> nodes;
    while (getline(cin, line)) {
        if (line.empty()) {
            continue;
        }
        istringstream ss(line);
        string node;
        ss >> node;
        string left;
        ss >> left;
        ss >> left;
        left = left.substr(1,3);
        string right;
        ss >> right;
        right = right.substr(0,3);
        // cout << node << " : " << left << ", " << right << endl;
        nodes.emplace(make_pair(node, Node(left, right)));
    }
    vector<string> currnodes;
    for (auto n : nodes) {
        if (n.first.back() == 'A') {
            currnodes.push_back(n.first);
        }
    }
    long ans = 1;
    for (auto n : currnodes) {
        int instruct = 0;
        int i = 0;
        unordered_set<string> seen;
        while (seen.find(n + to_string(instruct)) == seen.end()) {
            seen.insert(n + to_string(instruct));
            if (n.back() == 'Z') {
                break;
            }
            i++;
            if (instructions[instruct] == 'R') {
                n = nodes.at(n).right;
            } else {
                n = nodes.at(n).left;
            }
            instruct = (instruct + 1) % instructions.size();
        }
        cout << i << endl;
        ans = lcm(ans, i);
    }
    cout << ans << endl;
}