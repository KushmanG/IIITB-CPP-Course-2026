#include "tester_util.h"
// Private/protected opened up so the tests can check members the paper names.
#define private public
#define protected public
#define class struct   // members with no access keyword are private by default; this opens them too
#define main app_original_main
#include "student.cpp"
#undef main
static string ts(const vector<Tuple>& v) { string s; for (const Tuple& t : v) s += "(" + to_string(t.getKey()) + "," + to_string(t.getValue()) + ")"; return s; }
static vector<Tuple> T(const vector<pair<int,int>>& p) { vector<Tuple> v; for (auto& x : p) v.push_back(Tuple(x.first, x.second)); return v; }

static void runCase(const string& name, const string& label) {
    string in = readFile(name + ".in"), exp = readFile(name + ".expected");
    string got = runWithInput(in, []() { app_original_main(); });
    if (got == exp) { test(true, label, ""); return; }
    stringstream gs(got), es(exp); string gl, el; int line = 0;
    while (true) {
        bool hg = (bool)getline(gs, gl), he = (bool)getline(es, el); line++;
        if (!hg && !he) break;
        if (!hg) gl = "<missing>";
        if (!he) el = "<extra line>";
        if (gl != el) break;
    }
    test(false, "", label + ": output differs at line " + to_string(line) + ": got " + show(gl) + ", expected " + show(el));
}
int main() {
    cout << "--- Q6. main() input handling (full program run) ---" << endl;
    runCase("q6_a", "Paper example, one round, then n");
    runCase("q6_b", "Three rounds (y, Y, n): duplicates, an empty table, no matches");
    return 0;
}
