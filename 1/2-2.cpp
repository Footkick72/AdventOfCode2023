#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

int main() {
    string s;
    int ans = 0;
    unordered_map<string, string> nums = {
        {"one", "o1e"},
        {"two", "t2o"},
        {"three", "th3ee"},
        {"four", "f4ur"},
        {"five", "f5ve"},
        {"six", "s6x"},
        {"seven", "se7en"},
        {"eight", "ei8ht"},
        {"nine", "n9ne"}
    };
    while (getline(cin, s)) {
        for (const auto &o : nums) {
            size_t pos = s.find(o.first);
            while (pos != string::npos) {
                s.replace(pos, o.first.length(), o.second);
                pos = s.find(o.first);
            }
        }
        for (int i = 0; i < s.length(); i++) {
            if (isdigit(s[i])) {
                ans += 10*(s[i]-'0');
                break;
            }
        }
        for (int i = s.length()-1; i >= 0; i--) {
            if (isdigit(s[i])) {
                ans += s[i]-'0';
                break;
            }
        }
    }
    cout << ans << endl;
    return 0;
}