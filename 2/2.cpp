#include <iostream>

using namespace std;

int main() {
    string line;
    int r;
    int g;
    int b;
    int n;
    int ans = 0;
    while (getline(cin, line)) { 
        size_t i = line.find(':');
        r = 0;
        g = 0;
        b = 0;
        for (auto j = line.begin()+i; j != line.end(); j++) {
            if (isdigit(*j)) {
                n = *j - '0';
                j++;
                while (*j != ' ') {
                    n *= 10;
                    n += *j - '0';
                    j++;
                }
                if (*(j + 1) == 'r') {
                    r = max(r,n);
                }
                if (*(j + 1) == 'g') {
                    g = max(g,n);
                }
                if (*(j + 1) == 'b') {
                    b = max(b,n);
                }
            }
        }
        ans += r * g * b;
    }
    cout << ans << endl;
}