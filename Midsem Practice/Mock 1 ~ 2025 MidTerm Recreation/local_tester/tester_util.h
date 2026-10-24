// Shared helpers for the local tests. No change required.
#include <iostream>
#include <sstream>
#include <string>
#include <functional>
using namespace std;

inline void test(bool condition, const string& success, const string& failure) {
    if (condition) cout << "[PASS] " << success << endl;
    else cout << "[FAIL] " << failure << endl;
}

// Runs f with cout redirected; returns what f printed.
inline string captureOut(const function<void()>& f) {
    stringstream ss;
    streambuf* old = cout.rdbuf(ss.rdbuf());
    try { f(); } catch (...) { cout.rdbuf(old); throw; }
    cout.rdbuf(old);
    return ss.str();
}

// Returns the int thrown by f, or INT32_MIN if nothing / something else was thrown.
inline int thrownInt(const function<void()>& f) {
    try { f(); } catch (int x) { return x; } catch (...) { return -2147483647 - 1; }
    return -2147483647 - 1;
}

// Returns e.what() if f throws invalid_argument, "<none>" otherwise.
inline string thrownInvalidArg(const function<void()>& f) {
    try { f(); } catch (invalid_argument& e) { return e.what(); } catch (...) { return "<other>"; }
    return "<none>";
}

inline string show(const string& s) {   // make whitespace visible in failure messages
    string r;
    for (char c : s) { if (c == '\n') r += "\\n"; else if (c == ' ') r += "\xC2\xB7"; else r += c; }
    return "\"" + r + "\"";
}
