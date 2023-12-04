#include <iostream>
#include <sstream>
#include <unordered_set>

using namespace std;

int main() {
    string line;
    string word;
    int ans = 0;
    while (getline(cin, line)) {
        istringstream lss(line);
        lss >> word;
        lss >> word;
        unordered_set<int> winning;
        int score = 0;
        while (lss >> word) {
            if (isdigit(*word.begin())) {
                if (winning.find(stoi(word)) != winning.end()) {
                    if (score == 0) {
                        score = 1;
                    } else {
                        score *= 2;
                    }
                }
                winning.insert(stoi(word));
            }
        }
        ans += score;
    }
    cout << ans << endl;
}