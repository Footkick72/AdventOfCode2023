#include <iostream>
#include <sstream>
#include <map>

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
    auto i = instructions.begin();
    string node = "AAA";
    int ans = 0;
    while (node != "ZZZ") {
        ans++;
        Node &n = nodes.at(node);
        if (*i == 'R') {
            node = n.right;
        } else {
            node = n.left;
        }
        i++;
        if (i == instructions.end()) {
            i = instructions.begin();
        }
    }
    cout << ans << endl;
}