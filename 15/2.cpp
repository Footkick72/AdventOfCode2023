#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

struct Lens {
    string label;
    int focal;

    Lens(string s, int i) : label(s), focal(i) {}
};

int HASH(string s) {
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
    vector<Lens> boxes[256];
    while (getline(ss,step,',')) {
        if (step.find('=') != string::npos) {
            string l = step.substr(0, step.find('='));
            int f = stoi(step.substr(step.find('=')+1, step.size()));

            vector<Lens> &box = boxes[HASH(l)];

            bool found = false;
            for (auto &c : box) {
                if (c.label == l) {
                    c.focal = f;
                    found = true;
                    break;
                }
            }
            if (!found) box.emplace_back(l,f);
        }
        else {
            string l = step.substr(0, step.size()-1);
            vector<Lens> &box = boxes[HASH(l)];

            for (auto i = box.begin(); i != box.end(); i++) {
                if (i->label == l) {
                    box.erase(i);
                    break;
                }
            }
        }
    }
    int ans = 0;
    for (int boxi = 0; boxi < 256; boxi++) {
        auto &box = boxes[boxi];
        for (int lensi = 0; lensi < box.size(); lensi++) {
            ans += (1+boxi)*(1+lensi)*box[lensi].focal;
        }
    }
    cout << ans << endl;
    return 0;
}