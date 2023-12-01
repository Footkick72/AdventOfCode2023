#include <iostream>
#include <fstream>
#include <string>
#include <deque>

int main() {
    using namespace std;
    
    ifstream in("1/in.txt");
    string s;
    int ans = 0;
    deque<char> buff;
    while (getline(in, s)) {
        buff.clear();
        for (auto i = s.cbegin(); i != s.cend(); i++) {
            buff.push_back(*i);
            if ((buff.size()) > 5) {
                buff.pop_front();
            }
            if (isdigit(*(buff.end()-1))) {
                ans += 10*(*(buff.end()-1)-'0');
                break;
            }
            if (buff.size() >= 3) {
                string last3 = string(buff.cend()-3, buff.cend());
                if (last3 == "one") { ans += 10; break; }
                if (last3 == "two") { ans += 20; break; }
                if (last3 == "six") { ans += 60; break; }
            }
            if (buff.size() >= 4) {
                string last4 = string(buff.cend()-4, buff.cend());
                if (last4 == "four") { ans += 40; break; }
                if (last4 == "five") { ans += 50; break; }
                if (last4 == "nine") { ans += 90; break; }
            }
            if (buff.size() >= 5) {
                string last5 = string(buff.cend()-5, buff.cend());
                if (last5 == "three") { ans += 30; break; }
                if (last5 == "seven") { ans += 70; break; }
                if (last5 == "eight") { ans += 80; break; }
            }
        }
        
        buff.clear();
        for (auto i = s.crbegin(); i != s.crend(); i++) {
            buff.push_front(*i);
            if ((buff.size()) > 5) {
                buff.pop_back();
            }
            if (isdigit(*buff.begin())) {
                ans += (*buff.begin()-'0');
                break;
            }
            if (buff.size() >= 3) {
                string last3 = string(buff.cbegin(), buff.cbegin()+3);
                if (last3 == "one") { ans += 1; break; }
                if (last3 == "two") { ans += 2; break; }
                if (last3 == "six") { ans += 6; break; }
            }
            if (buff.size() >= 4) {
                string last4 = string(buff.cbegin(), buff.cbegin()+4);
                if (last4 == "four") { ans += 4; break; }
                if (last4 == "five") { ans += 5; break; }
                if (last4 == "nine") { ans += 9; break; }
            }
            if (buff.size() >= 5) {
                string last5 = string(buff.cbegin(), buff.cbegin()+5);
                if (last5 == "three") { ans += 3; break; }
                if (last5 == "seven") { ans += 7; break; }
                if (last5 == "eight") { ans += 8; break; }
            }
        }
    }
    cout << ans << endl;
    return 0;
}