#include <iostream>
#include <sstream>

using namespace std;

int main() {
    string line;
    int area = 0;
    int y = 0;
    int len = 0;
    while (getline(cin,line)) {
        istringstream ss(line);
        char d;
        ss >> d;
        int l;
        ss >> l;
        len += l;
        if (d == 'R') area += y*l;
        if (d == 'U') y+=l;
        if (d == 'L') area -= y*l;
        if (d == 'D') y-=l;
    }
    cout << area + len/2 + 1 << endl;
    return 0;
}