#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

#include "driver.h"
static void runSample(const string& name) {
    string logf = name + "_run_log.txt";
    remove(logf.c_str());
    ifstream in(name + ".in");
    string got = captureOut([&]() { runDriver(in, logf); });
    string exp = readFile(name + ".expected");
    string gotLog = readFile(logf), expLog = readFile(name + ".log.expected");
    remove(logf.c_str());
    for (int k = 0; k < 2; k++) {
        const string& g = k ? gotLog : got; const string& e = k ? expLog : exp;
        if (g == e) continue;
        stringstream gs(g), es(e); string gl, el; int line = 0;
        while (true) {
            bool hg = (bool)getline(gs, gl), he = (bool)getline(es, el); line++;
            if (!hg && !he) break;
            if (!hg) gl = "<missing>";
            if (!he) el = "<extra line>";
            if (gl != el) break;
        }
        test(false, "", name + (k ? " LOG FILE" : " STDOUT") + " differs at line " + to_string(line) + ": got " + show(gl) + ", expected " + show(el));
        return;
    }
    test(true, name + " end-to-end: stdout and log file match", "");
}
int main() {
    cout << "--- 9. End-to-end driver run: " << SAMPLE << " ---" << endl;
    runSample(SAMPLE);
    return 0;
}
