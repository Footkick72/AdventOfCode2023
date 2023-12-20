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

    mod(string n, vector<string> r) : name(n), recipients(r) {}

    virtual ~mod() {}
};

struct flipflop : public mod {
    bool state;

    flipflop(string n, vector<string> r) : mod(n,r), state(false) {}

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

    deque<tuple<string,string,bool>> pulses;
    int low = 0;
    int high = 0;
    for (int _ = 0; _ < 1000; _++) {
        low++;
        for (auto b : broadcast) pulses.emplace_back("broadcaster",b,false);
        while (!pulses.empty()) {
            auto [s,n,p] = pulses.front();
            pulses.pop_front();

            if (p) high++; else low++;

            if (mods.find(n) == mods.end()) continue;

            auto r = mods.at(n).get();
            if (r->recieve_pulse(s,p)) {
                for (auto o : r->recipients) {
                    pulses.emplace_back(n, o, r->get_output());
                }
            }
        }
    }
    cout << (long)low * (long)high << endl;
    return 0;
}