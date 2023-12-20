#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <deque>

using namespace std;

class mod {
    public:
    string name;
    vector<string> recipients;

    virtual void connect_input(string n) = 0;
    virtual bool recieve_pulse(string s, bool p) = 0;
    virtual bool get_output() = 0;
    virtual void reset() = 0;

    mod(string n, vector<string> r) : name(n), recipients(r) {}

    virtual ~mod() {}
};

struct flipflop : public mod {
    bool state;

    flipflop(string n, vector<string> r) : mod(n,r), state(false) {}

    void reset() override {
        state = false;
    }

    void connect_input(string n) override {}

    bool recieve_pulse(string s, bool p) override {
        if (!p) state = !state; 
        return !p;
    }

    bool get_output() override {
        return state;
    }
};

struct conjunct: public mod {
    map<string,bool> state;

    conjunct(string n, vector<string> r) : mod(n,r), state({}) {}

    void reset() override {
        for (auto p : state) {
            state[p.first] = false;
        }
    }

    void connect_input(string n) override {
        state[n] = false;
    }

    bool recieve_pulse(string s, bool p) override {
        state[s] = p;
        return true;
    }

    bool get_output() override {
        for (auto b : state) {
            if (!b.second) return true;
        }
        return false;
    }
};

int main() {
    string line;
    vector<string> broadcast;
    map<string,unique_ptr<mod>> mods;
    while (getline(cin,line)) {
        istringstream ss(line);
        string name;
        ss >> name;
        string _;
        ss >> _;
        vector<string> rs;
        string r;
        while (getline(ss,r,',')) {
            rs.push_back(r.substr(1));
        }

        if (name[0] == 'b') {
            broadcast = rs;
        } else if (name[0] == '&') {
            mods.emplace(name.substr(1),new conjunct(name.substr(1),rs));
        } else if (name[0] == '%') {
            mods.emplace(name.substr(1),new flipflop(name.substr(1),rs));
        }
    }

    // register inputs to conjunctions
    for (auto &[n1,p1] : mods) {
        for (auto &[n2,p2] : mods) {
            if (find(p2.get()->recipients.begin(), p2.get()->recipients.end(), p1.get()->name) != p2.get()->recipients.end()) {
                // p1 is a recipient of p2, i.e. p2 is an input of p1
                p1.get()->connect_input(p2.get()->name);
            }
        }
    }

    string outputnand;
    for (auto &[n,p] : mods) {
        if (find(p.get()->recipients.begin(),p.get()->recipients.end(),"rx") != p.get()->recipients.end()) {
            outputnand = n;
            break;
        }
    }
    vector<string> contributers;
    for (auto &[n,p] : mods) {
        if (find(p.get()->recipients.begin(),p.get()->recipients.end(),outputnand) != p.get()->recipients.end()) {
            contributers.push_back(n);
        }
    }

    vector<int> answers(contributers.size(),0);
    deque<tuple<string,string,bool>> pulses;
    int a = 0;
    bool run = true;
    while (run) {
        a++;
        for (auto b : broadcast) pulses.emplace_back("broadcaster",b,false);
        while (!pulses.empty()) {
            auto [s,n,p] = pulses.front();
            pulses.pop_front();
            run = false;
            for (int ci = 0; ci < contributers.size(); ci++) {
                if (s == contributers[ci] && p) {
                    answers[ci] = a;
                }
                if (answers[ci] == 0) run = true;
            }

            if (mods.find(n) == mods.end()) continue;

            auto r = mods.at(n).get();
            if (r->recieve_pulse(s,p)) {
                for (auto o : r->recipients) {
                    pulses.emplace_back(n, o, r->get_output());
                }
            }
        }
    }
    long ans = 1;
    for (auto c : answers) ans *= c;
    cout << ans << endl;
    return 0;
}