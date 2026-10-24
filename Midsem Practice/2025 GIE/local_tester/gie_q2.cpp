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

struct Probe : public JoinOperator {
    Probe(vector<Tuple>& l, vector<Tuple>& r) : JoinOperator(l, r) {}
    void execute() override { result.push_back(Tuple(9, 9)); }
};
int main() {
    cout << "--- Q2. JoinOperator ---" << endl;
    test(is_abstract<JoinOperator>::value, "JoinOperator is abstract (pure virtual execute)", "JoinOperator is not abstract: execute() must be pure virtual (= 0)");
    test(has_virtual_destructor<JoinOperator>::value, "JoinOperator has a virtual destructor", "JoinOperator destructor must be virtual");
    vector<Tuple> l = T({{1, 10}, {2, 20}}), r = T({{2, 200}});
    Probe p(l, r);
    bool init = p.left.size() == 2 && p.right.size() == 1 && p.result.empty() && p.getResult().empty();
    p.execute();
    vector<Tuple> copy = p.getResult();
    copy.push_back(Tuple(0, 0));
    test(init && p.getResult().size() == 1 && ts(p.getResult()) == "(9,9)",
         "Constructor copies left/right, result starts empty, getResult() returns by value",
         "JoinOperator constructor/getResult wrong");
    return 0;
}
