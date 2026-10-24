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
    cout << "--- Q5. HashJoin ---" << endl;
    vector<Tuple> l = T({{1, 10}, {2, 20}, {3, 30}, {2, 25}}), r = T({{2, 200}, {3, 300}, {4, 400}, {1, 250}});
    HashJoin j(l, r);
    test(j.ht != nullptr && j.ht->buckets.size() == 5 && j.ht->getBucket(2).empty(),
         "Constructor allocates a HashTable with left.size()+1 buckets (empty until execute)",
         "ht should point to a new HashTable of size left.size()+1 = 5, still empty");
    j.populateHashTable();
    test(j.ht->getBucket(2).size() == 2 && j.ht->getBucket(1).size() == 1 && j.result.empty(),
         "populateHashTable inserts every left tuple (and produces no result rows)",
         "populateHashTable wrong");
    HashJoin j2(l, r); j2.execute();
    string exp = "(2,220)(2,225)(3,330)(1,260)";
    test(ts(j2.getResult()) == exp, "execute: right-table order, bucket order inside", "Got " + ts(j2.getResult()) + " expected " + exp);
    vector<Tuple> l3 = T({{6, 1}, {1, 2}, {6, 3}, {11, 4}}), r3 = T({{6, 10}, {11, 20}, {1, 30}, {16, 40}});
    JoinOperator* p = new HashJoin(l3, r3);
    p->execute();
    string got = ts(p->getResult());
    delete p;
    string exp3 = "(6,11)(6,13)(11,24)(1,32)";   // 5 buckets: keys 6,1,11,16 collide in bucket 1
    test(got == exp3, "Colliding keys in one bucket: only equal keys join; works through JoinOperator*", "Got " + got + " expected " + exp3);
    return 0;
}
