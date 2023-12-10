#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

struct Pipe {
    char type;
    int i,j;
    Pipe *n, *s, *e, *w;

    Pipe(char t, int i, int j) : type(t), i(i), j(j), n(nullptr), s(nullptr), e(nullptr), w(nullptr) {};
};

pair<bool, vector<Pipe*>> getloop(Pipe* start, Pipe* next) {
    assert(start && next);
    Pipe* p = next;
    vector<Pipe*> path = {start};
    while (p != start || path.size() == 0) {
        if (p->n && p->n->s && p->n != path.back()) {
            path.push_back(p);
            p = p->n;
        } else if (p->s && p->s->n && p->s != path.back()) {
            path.push_back(p);
            p = p->s;
        } else if (p->e && p->e->w && p->e != path.back()) {
            path.push_back(p);
            p = p->e;
        } else if (p->w && p->w->e && p->w != path.back()) {
            path.push_back(p);
            p = p->w;
        } else {
            // dead end, initial start route wasn't correct
            return {false, path};
        }
    }
    return {true, path};
}

int main() {
    vector<vector<Pipe*>> grid;
    string line;
    Pipe* startpipe;
    int i = 0, j = 0;
    while (getline(cin, line)) {
        vector<Pipe*> row;
        j = 0;
        for (auto c : line) {
            Pipe* p = new Pipe(c,i,j);
            row.push_back(p);
            if (c == 'S') {
                startpipe = p;
            }
            j++;
        }
        i++;
        grid.push_back(row);
    }
    
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            bool n, e, s, w;
            if (grid[i][j]->type == '|') {
                n=true; e=false; s=true; w=false;
            } else if (grid[i][j]->type == '-') {
                n=false; e=true; s=false; w=true;
            } else if (grid[i][j]->type == 'L') {
                n=true; e=true; s=false; w=false;
            } else if (grid[i][j]->type == 'J') {
                n=true; e=false; s=false; w=true;
            } else if (grid[i][j]->type == '7') {
                n=false; e=false; s=true; w=true;
            } else if (grid[i][j]->type == 'F') {
                n=false; e=true; s=true; w=false;
            } else if (grid[i][j]->type == '.') {
                n=false; e=false; s=false; w=false;
            } else {
                n=true; e=true; s=true; w=true;
            }

            if (n) {
                if (i > 0) grid[i][j]->n = grid[i-1][j];
            }
            if (e) {
                if (j < grid[i].size()-1) grid[i][j]->e = grid[i][j+1];
            }
            if (s) {
                if (i < grid.size()-1) grid[i][j]->s = grid[i+1][j];
            }
            if (w) {
                if (j > 0) grid[i][j]->w = grid[i][j-1];
            }
        }
    }

    for (auto n : {startpipe->n, startpipe->e, startpipe->s, startpipe->w}) {
        if (!n) continue;
        if (startpipe == n->n || startpipe == n->e || startpipe == n->s || startpipe == n->w) {
            auto p = getloop(startpipe,n);
            if (p.first) {
                auto path = p.second;
                int area = 0;
                for (int i = 0; i < path.size(); i++) {
                    int j = (i+1)%path.size();
                    auto p0 = path[i];
                    auto p1 = path[j];
                    area += p1->i*p0->j - p0->i*p1->j;
                }
                area = abs(area)/2;
                cout << area - p.second.size()/2 + 1 << endl;
                break;
            }
        }
    }
    
    return 0;
}