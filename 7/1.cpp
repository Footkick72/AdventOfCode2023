#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

int getcardtype(const string &card, const map<char,int> &charscores) {
    int charcounts[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
    for (auto c : card) {
        charcounts[charscores.at(c)]++;
    }
    // cout << card << ": [";
    int m1 = 0;
    int m2 = 0;
    for (auto x : charcounts) {
        // cout << x << ", ";
        if (x > m1) {
            m2 = m1;
            m1 = x;
        } else if (x > m2) {
            m2 = x;
        }
    }
    // cout << "]" << endl;
    if (m1 == 5) {
        return 6;
    } else if (m1 == 4) {
        return 5;
    } else if (m1 == 3 && m2 == 2) {
        return 4;
    } else if (m1 == 3) {
        return 3;
    } else if (m1 == 2 && m2 == 2) {
        return 2;
    } else if (m1 == 2) {
        return 1;
    } else {
        return 0;
    }
}

struct PokerComp {
    bool operator()(const string &s1, const string &s2) const {
        assert(s1 != s2);

        static const map<char, int> charscores = {
            {'2', 0},
            {'3', 1},
            {'4', 2},
            {'5', 3},
            {'6', 4},
            {'7', 5},
            {'8', 6},
            {'9', 7},
            {'T', 8},
            {'J', 9},
            {'Q', 10},
            {'K', 11},
            {'A', 12},
        };
        
        int t1 = getcardtype(s1, charscores);
        int t2 = getcardtype(s2, charscores);
        // cout << s1 << ": " << t1 << endl;
        // cout << s2 << ": " << t2 << endl; 
        if (t1 != t2) {
            return t1 < t2;
        } else {
            for (auto i = 0; i < 5; i++) {
                if (charscores.at(s1[i]) != charscores.at(s2[i])) {
                    return charscores.at(s1[i]) < charscores.at(s2[i]);
                }
            }
        }
        return true;
    }
};


int main() {
    string line;
    string word;
    int score; 

    map<string,int,PokerComp> scores;
    
    while (getline(cin, line)) {
        istringstream ss(line);
        ss >> word;
        ss >> score;
        scores[word] = score;
    }

    int ans = 0;
    int i = 0;
    for (auto s : scores) {
        ans += ++i * s.second;
    }
    cout << ans << endl;
}