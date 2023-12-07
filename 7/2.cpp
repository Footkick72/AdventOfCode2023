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
    int m1 = 0;
    int m2 = 0;
    for (auto x = charcounts+1; x < charcounts+13; x++) {
        if (*x > m1) {
            m2 = m1;
            m1 = *x;
        } else if (*x > m2) {
            m2 = *x;
        }
    }
    if (charcounts[0] + m1 == 5) {
        return 6;
    } else if (charcounts[0] + m1 == 4) {
        return 5;
    } else if (charcounts[0] + m1 == 3 && m2 == 2) {
        return 4;
    } else if (charcounts[0] + m1 == 3) {
        return 3;
    } else if (m1 == 2 && charcounts[0] + m2 == 2) {
        return 2;
    } else if (charcounts[0] + m1 == 2) {
        return 1;
    } else {
        return 0;
    }
}

struct PokerComp {
    bool operator()(const string &s1, const string &s2) const {
        assert(s1 != s2);

        static const map<char, int> charscores = {
            {'J', 0},
            {'2', 1},
            {'3', 2},
            {'4', 3},
            {'5', 4},
            {'6', 5},
            {'7', 6},
            {'8', 7},
            {'9', 8},
            {'T', 9},
            {'Q', 10},
            {'K', 11},
            {'A', 12},
        };
        
        int t1 = getcardtype(s1, charscores);
        int t2 = getcardtype(s2, charscores);
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