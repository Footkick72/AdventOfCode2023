#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

int main() {
    string line;
    long elem;
    vector<vector<long>> sequences;
    while (getline(cin, line)) {
        vector<long> seq;
        istringstream ss(line);
        while (ss >> elem) {
            seq.push_back(elem);
        }
        sequences.push_back(seq);
    }
    int ans = 0;
    for (const auto &seq : sequences) {
        vector<long> consecutiveDiffs;
        vector<long> iter = seq;
        vector<long> iter2;
        bool done = false;
        while (!done) {
            for (auto i = iter.begin(); i != iter.end()-1; i++) {
                iter2.emplace_back(*(i+1)-*i);
            }
            iter = iter2;
            iter2.clear();
            consecutiveDiffs.push_back(iter.back());
            
            done = true;
            for (auto i : iter) {
                if (i != 0) {
                    done = false;
                    break;
                }
            }
        }
        
        long inc = 0;
        for (auto i = consecutiveDiffs.rbegin(); i != consecutiveDiffs.rend(); i++) {
            inc = *i + inc;
        }
        ans += seq.back() + inc;
    }
    cout << ans << endl;
    return 0;
}