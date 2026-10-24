// Shared helpers for the local tests. No change required.
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <cstdio>
using namespace std;

inline void test(bool condition, const string& success, const string& failure) {
    if (condition) cout << "[PASS] " << success << endl;
    else cout << "[FAIL] " << failure << endl;
}

inline string captureOut(const function<void()>& f) {
    stringstream ss;
    streambuf* old = cout.rdbuf(ss.rdbuf());
    try { f(); } catch (...) { cout.rdbuf(old); throw; }
    cout.rdbuf(old);
    return ss.str();
}

// what() of the runtime_error thrown by f; "<none>" if nothing thrown, "<other>" for anything else
inline string thrownWhat(const function<void()>& f) {
    try { f(); } catch (runtime_error& e) { return e.what(); } catch (...) { return "<other>"; }
    return "<none>";
}

inline string readFile(const string& f) { ifstream in(f); stringstream ss; ss << in.rdbuf(); return ss.str(); }

inline string show(const string& s) {
    string r;
    for (char c : s) { if (c == '\n') r += "\\n"; else r += c; }
    return "\"" + r + "\"";
}

inline bool near(double a, double b) { return a - b < 1e-6 && b - a < 1e-6; }
