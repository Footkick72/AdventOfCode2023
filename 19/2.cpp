#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <tuple>

using namespace std;

struct part {
    int minx, maxx, minm, maxm, mina, maxa, mins, maxs;

    part() : minx(1), maxx(4001), minm(1), maxm(4001), mina(1), maxa(4001), mins(1), maxs(4001) {}
    part(const part &o) {
        minx = o.minx;
        maxx = o.maxx;
        minm = o.minm;
        maxm = o.maxm;
        mina = o.mina;
        maxa = o.maxa;
        mins = o.mins;
        maxs = o.maxs;
    }
    part(const part &&o) {
        minx = o.minx;
        maxx = o.maxx;
        minm = o.minm;
        maxm = o.maxm;
        mina = o.mina;
        maxa = o.maxa;
        mins = o.mins;
        maxs = o.maxs;
    }
};

struct workflow_branch {
    bool less;
    string comp;
    int val;
    string res;

    workflow_branch(bool l, string c, int v, string r) : less(l), comp(c), val(v), res(r) {}

    vector<pair<bool,part>> process(part &p) {
        if (comp == "") return {{true,p}};
        part p1(p); // doesn't get mapped
        part p2(p); // does get mapped
        if (less) {
            if (comp == "x") {
                p1.minx = max(p1.minx,val);
                p2.maxx = min(p2.maxx,val);
            } else if (comp == "m") {
                p1.minm = max(p1.minm,val);
                p2.maxm = min(p2.maxm,val);
            } else if (comp == "a") {
                p1.mina = max(p1.mina,val);
                p2.maxa = min(p2.maxa,val);
            } else if (comp == "s") {
                p1.mins = max(p1.mins,val);
                p2.maxs = min(p2.maxs,val);
            }
        } else {
            if (comp == "x") {
                p1.maxx = min(p1.maxx,val+1);
                p2.minx = max(p2.minx,val+1);
            } else if (comp == "m") {
                p1.maxm = min(p1.maxm,val+1);
                p2.minm = max(p2.minm,val+1);
            } else if (comp == "a") {
                p1.maxa = min(p1.maxa,val+1);
                p2.mina = max(p2.mina,val+1);
            } else if (comp == "s") {
                p1.maxs = min(p1.maxs,val+1);
                p2.mins = max(p2.mins,val+1);
            }
        }
        return {{false,p1},{true,p2}};
    }
};

struct workflow {
    string name;
    vector<workflow_branch> branches;

    workflow(string n, vector<workflow_branch> b) : name(n), branches(b) {}

    vector<pair<string,part>> process(part &p) {
        vector<pair<string,part>> processed_parts;
        vector<part> unprocessed_parts = {p};
        int i = 0;
        while (!unprocessed_parts.empty()) {
            part p(unprocessed_parts.back());
            unprocessed_parts.pop_back();
            for (auto p : branches[i].process(p)) {
                if (p.first) {
                    processed_parts.emplace_back(branches[i].res, p.second);
                } else {
                    unprocessed_parts.emplace_back(p.second);
                }
            }
            i++;
        }
        return processed_parts;
    }
};

int main() {
    string line;
    map<string,workflow> workflows;
    bool t = false;
    while (getline(cin,line)) {
        if (line.empty()) {
            break;
        }
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
        workflows.emplace(name,workflow(name,branches));
    }

    part start;
    long ans = 0;
    vector<pair<string,part>> parts = {{"in", start}};
    while (!parts.empty()) {
        auto p = parts.back();
        parts.pop_back();
        auto w = workflows.at(p.first);

        auto np = w.process(p.second);
        for (auto p1 : np) {
            if (p1.second.maxx <= p1.second.minx || p1.second.maxm <= p1.second.minm || p1.second.maxa <= p1.second.mina || p1.second.maxs <= p1.second.mins) {
                continue;
            }
            if (p1.first == "A") {
                long prod = (p1.second.maxx - p1.second.minx);
                prod *= (p1.second.maxm - p1.second.minm);
                prod *= (p1.second.maxa - p1.second.mina);
                prod *= (p1.second.maxs - p1.second.mins);
                ans += prod;
            } else if (p1.first == "R") {
            } else {
                parts.emplace_back(p1);
            }
        }
    }
    cout << ans << endl;
    return 0;
}