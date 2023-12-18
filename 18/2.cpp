#include <iostream>
#include <sstream>

using namespace std;

int main() {
    string line;
    long long area = 0;
    long long y = 0;
    long long len = 0;
    while (getline(cin,line)) {
        istringstream ss(line);
        char d1;
        ss >> d1;
        long long l;
        ss >> l;
        string code;
        ss >> code;
        l = stoi(code.substr(2,5), nullptr, 16);
        int d = code[7]-'0';
        len += l;
        if (d == 0) area += y*l;
        if (d == 3) y+=l;
        if (d == 2) area -= y*l;
        if (d == 1) y-=l;
    }
    cout << area + len/2 + 1 << endl;
    return 0;
}