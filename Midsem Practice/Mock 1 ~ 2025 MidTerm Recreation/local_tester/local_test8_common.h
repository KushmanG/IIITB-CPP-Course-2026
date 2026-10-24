// Shared body of local_test8a/8b (each sample runs in a fresh process so paper IDs restart at 1).
#include "tester_util.h"
#include <fstream>
#define main app_original_main
#include "student.cpp"
#undef main
#include "driver.h"

static string readFile(const string& f) { ifstream in(f); stringstream ss; ss << in.rdbuf(); return ss.str(); }

static void runSample(const string& name) {
    ifstream in(name + ".in");
    string got = captureOut([&]() { runDriver(in); });
    string exp = readFile(name + ".expected");
    if (got == exp) { test(true, name + " end-to-end output matches", ""); return; }
    // find first differing line
    stringstream g(got), e(exp); string gl, el; int line = 0;
    while (true) {
        bool hg = (bool)getline(g, gl), he = (bool)getline(e, el); line++;
        if (!hg && !he) break;
        if (!hg) gl = "<missing>";
        if (!he) el = "<extra line>";
        if (gl != el) { test(false, "", name + " differs at output line " + to_string(line) + ": got " + show(gl) + ", expected " + show(el)); return; }
    }
    test(false, "", name + " differs (trailing whitespace?)");
}

int main() {
    cout << "--- 8. End-to-end driver run: " << SAMPLE << " (Sample_Runs.pdf) ---" << endl;
    runSample(SAMPLE);
    return 0;
}
