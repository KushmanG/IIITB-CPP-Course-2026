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
    cout << "--- Q3. NestedLoopJoin ---" << endl;
    vector<Tuple> l = T({{1, 10}, {2, 20}, {3, 30}, {2, 25}}), r = T({{2, 200}, {3, 300}, {4, 400}, {1, 250}});
    NestedLoopJoin j(l, r); j.execute();
    string exp = "(1,260)(2,220)(3,330)(2,225)";
    test(ts(j.getResult()) == exp, "Paper example: rows in left-outer / right-inner order", "Got " + ts(j.getResult()) + " expected " + exp);
    vector<Tuple> l2 = T({{5, 1}, {5, 2}, {7, 0}}), r2 = T({{5, 10}, {5, 20}, {5, 30}});
    NestedLoopJoin j2(l2, r2); j2.execute();
    string exp2 = "(5,11)(5,21)(5,31)(5,12)(5,22)(5,32)";
    test(ts(j2.getResult()) == exp2, "Duplicate keys: m*n output rows", "Got " + ts(j2.getResult()) + " expected " + exp2);
    vector<Tuple> e;
    vector<Tuple> l3 = T({{1, 1}}), r3 = T({{2, 2}});
    NestedLoopJoin j3(e, r3), j4(l3, e), j5(l3, r3);
    j3.execute(); j4.execute(); j5.execute();
    test(j3.getResult().empty() && j4.getResult().empty() && j5.getResult().empty(), "Empty table or no common key gives an empty result", "Expected empty results");
    return 0;
}
