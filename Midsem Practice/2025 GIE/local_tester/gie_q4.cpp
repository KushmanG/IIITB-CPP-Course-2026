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
    cout << "--- Q4. HashTable ---" << endl;
    HashTable h(5);
    test(h.buckets.size() == 5 && h.buckets[0].empty() && h.hash(7) == 2 && h.hash(5) == 0 && h.hash(3) == 3,
         "Constructor makes `size` empty buckets; hash(key) = key % size",
         "HashTable(5): expected 5 empty buckets, hash(7)=2, hash(5)=0, hash(3)=3");
    h.insert(2, 20); h.insert(7, 70); h.insert(2, 25); h.insert(4, 40);
    vector<HashEntry>& b = h.getBucket(12);   // 12 % 5 = 2
    bool order = b.size() == 3 && b[0].getValue() == 20 && b[1].getValue() == 70 && b[2].getValue() == 25;
    b.push_back(HashEntry(2, 99));
    test(order && h.getBucket(2).size() == 4 && h.getBucket(4).size() == 1 && h.getBucket(1).empty(),
         "insert goes to bucket hash(key) in order; getBucket returns a REFERENCE",
         "insert/getBucket wrong (is getBucket returning by reference?)");
    HashTable z(0);
    string w1 = thrownWhat([&]() { z.insert(1, 1); });
    string w2 = thrownWhat([&]() { z.getBucket(1); });
    test(w1 == "No entry" && w2 == "No entry", "Zero buckets: hash throws runtime_error(\"No entry\")", "Expected \"No entry\" twice, got " + w1 + " / " + w2);
    return 0;
}
