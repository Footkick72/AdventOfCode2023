#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

using namespace std;

int main() {
    string line;
    string word;
    int ans = 0;
    unordered_map<int, int> copies;
    while (getline(cin, line)) {
        istringstream lss(line);
        lss >> word;
        lss >> word;
        int cardnum = stoi(word);
        unordered_set<int> winning;
        int score = 0;
        while (lss >> word) {
            if (isdigit(*word.begin())) {
                if (winning.find(stoi(word)) != winning.end()) { score++; }
                winning.insert(stoi(word));
            }
        }
        int c = 1;
        if (copies.find(cardnum) != copies.end()) {
            c += copies[cardnum];
        }
        for (int i = cardnum + 1; i <= cardnum + score; i++) {
            copies[i] += c;
        }
        ans += c;
    }
    cout << ans << endl;
}