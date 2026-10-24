// Shared helpers for the local tests. No change required.
// All standard headers are included up front, so a test may "#define private public"
// before including student.cpp without breaking the library.
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <array>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <functional>
#include <memory>
#include <stdexcept>
#include <exception>
#include <utility>
#include <iterator>
#include <limits>
#include <type_traits>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstring>
using namespace std;

inline void test(bool condition, const string& success, const string& failure) {
    if (condition) cout << "[PASS] " << success << endl;
    else cout << "[FAIL] " << failure << endl;
}

// Runs f with cout redirected; returns what f printed (restores cout even if f throws).
inline string captureOut(const function<void()>& f) {
    stringstream ss;
    streambuf* old = cout.rdbuf(ss.rdbuf());
    try { f(); } catch (...) { cout.rdbuf(old); throw; }
    cout.rdbuf(old);
    return ss.str();
}

// Runs f with cin reading `input` and cout captured; returns the output.
inline string runWithInput(const string& input, const function<void()>& f) {
    stringstream in(input);
    streambuf* oldIn = cin.rdbuf(in.rdbuf());
    string out;
    try { out = captureOut(f); } catch (...) { cin.rdbuf(oldIn); throw; }
    cin.rdbuf(oldIn);
    cin.clear();
    return out;
}

// what() of the std::exception thrown by f; "<none>" if nothing thrown, "<other>" for non-std exceptions
inline string thrownWhat(const function<void()>& f) {
    try { f(); } catch (exception& e) { return e.what(); } catch (...) { return "<other>"; }
    return "<none>";
}

inline string readFile(const string& f) { ifstream in(f); stringstream ss; ss << in.rdbuf(); return ss.str(); }

inline vector<string> readLines(const string& f) {
    vector<string> v; ifstream in(f); string l;
    while (getline(in, l)) { if (!l.empty() && l.back() == '\r') l.pop_back(); v.push_back(l); }
    return v;
}

inline string show(const string& s) {
    string r;
    for (char c : s) { if (c == '\n') r += "\\n"; else r += c; }
    return "\"" + r + "\"";
}

inline bool near(double a, double b) { return fabs(a - b) < 1e-6; }
