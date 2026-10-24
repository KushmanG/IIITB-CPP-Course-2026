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

int main() {
    cout << "--- Q1. Tuple and HashEntry ---" << endl;
    const Tuple t(2, 225);
    test(t.getKey() == 2 && t.getValue() == 225, "Tuple(2,225): getKey()/getValue() (callable on const)", "Tuple getters wrong");
    HashEntry h(3, 35);
    test(h.getKey() == 3 && h.getValue() == 35, "HashEntry(3,35): getKey()/getValue()", "HashEntry getters wrong");
    Tuple neg(-4, 0);
    test(neg.getKey() == -4 && neg.getValue() == 0, "Tuple stores negative / zero values unchanged", "Tuple(-4,0) wrong");
    return 0;
}
