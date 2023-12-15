#include <iostream>
#include <sstream>

using namespace std;


int HASH(string &s) {
    int v = 0;
    for (const char &c : s) {
        v += c;
        v *= 17;
        v %= 256;
    }
    return v;
}

int main() {
    string line;
    cin >> line;
    istringstream ss(line);
    string step;
    int ans = 0;
    while (getline(ss,step,',')) {
        ans += HASH(step);
    }
    cout << ans << endl;
    return 0;
}