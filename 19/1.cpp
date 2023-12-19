#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <tuple>

using namespace std;

struct part {
    int x,m,a,s;

    part(int x, int m, int a, int s) : x(x), m(m), a(a), s(s) {}
};

struct workflow_branch {
    bool less;
    string comp;
    int val;
    string res;

    workflow_branch(bool l, string c, int v, string r) : less(l), comp(c), val(v), res(r) {}

    string process(part &p) {
        int v;
        if (comp == "x") v = p.x;
        else if (comp == "m") v = p.m;
        else if (comp == "a") v = p.a;
        else if (comp == "s") v = p.s;
        else if (comp == "") return res;
        if ((less && v < val) || (!less && v > val)) return res;
        return "-1";
    }
};

struct workflow {
    string name;
    vector<workflow_branch> branches;

    workflow(string n, vector<workflow_branch> b) : name(n), branches(b) {}

    string process(part &p) {
        string res = "-1";
        int i = 0;
        while (res == "-1" && i < branches.size()) {
            res = branches[i].process(p);
            i++;
        }
        return res;
    }
};

int main() {
    string line;
    map<string,workflow> workflows;
    vector<part> parts;
    bool t = false;
    while (getline(cin,line)) {
        if (line.empty()) {
            t = true;
            continue;
        }
        if (t) {
            int prop[4] = {0,0,0,0};
            istringstream ss(line);
            string word;
            getline(ss,word,'=');
            for (int &var : prop) {
                getline(ss,word,'=');
                for (auto c : word) {
                    if (!isdigit(c)) break;
                    var = var * 10 + c-'0';
                }
            }
            parts.emplace_back(prop[0],prop[1],prop[2],prop[3]);
            // cout << prop[0] << ',' << prop[1] << ',' << prop[2] << ',' << prop[3] << endl;
        } else {
            istringstream ss(line.substr(line.find('{')+1));
            string name = line.substr(0,line.find('{'));
            string word;
            vector<workflow_branch> branches;
            while (getline(ss,word,',')) {
                if (word.find('}') != string::npos) word = word.substr(0,word.size()-1);
                istringstream ss2(word);
                string p;
                getline(ss2,p,':');
                string r;
                getline(ss2,r,':');
                if (p.find('<') != string::npos) {
                    istringstream ss3(p);
                    string c;
                    getline(ss3,c,'<');
                    int v;
                    ss3 >> v;
                    branches.emplace_back(true,c,v,r);
                } else if (p.find('>') != string::npos) {
                    istringstream ss3(p);
                    string c;
                    getline(ss3,c,'>');
                    int v;
                    ss3 >> v;
                    branches.emplace_back(false,c,v,r);
                } else {
                    branches.emplace_back(false,"",-1,p);
                }
            }
            // for (auto b : branches) {
            //     cout << b.less << ',' << b.comp << ',' << b.val << ',' << b.res << endl;
            // }
            workflows.emplace(name,workflow(name,std::move(branches)));
        }
    }

    int ans = 0;
    for (auto &p : parts) {
        string flow = "in";
        while (flow != "A" && flow != "R") {
            flow = workflows.at(flow).process(p);
        }
        if (flow == "A") {
            ans += p.x + p.m + p.a + p.s;
        }
    }
    cout << ans << endl;
    return 0;
}