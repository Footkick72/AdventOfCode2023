#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream in("1/in.txt");
    string s;
    int ans = 0;
    while (getline(in, s)) {
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