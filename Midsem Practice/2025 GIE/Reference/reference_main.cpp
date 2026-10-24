// Reference solution — 2025 GIE (JOIN operators). Open only after attempting.
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;


/* define class Tuple below*/
class Tuple {
    int key;
    int value;
public:
    Tuple(int key, int value) : key(key), value(value) {}
    int getKey() const { return key; }
    int getValue() const { return value; }
};

/* define class HashEntry below*/
class HashEntry {
    int key;
    int value;
public:
    HashEntry(int key, int value) : key(key), value(value) {}
    int getKey() const { return key; }
    int getValue() const { return value; }
};



/* ========== DO NOT MODIFY BELOW ==== READONLY CODE ==== */
void printTuples(const vector<Tuple>& tuples) {
    for (size_t i = 0; i < tuples.size(); i++) {
        cout << "("
                  << tuples[i].getKey() << ", "
                  << tuples[i].getValue() << ")\n";
    }
}
/* ========== END OF READONLY CODE ============= */



/*
 * define class JoinOperator below
 */
class JoinOperator {
protected:
    vector<Tuple> left;
    vector<Tuple> right;
    vector<Tuple> result;
public:
    JoinOperator(vector<Tuple>& left, vector<Tuple>& right) : left(left), right(right) {}
    virtual ~JoinOperator() {}
    virtual void execute() = 0;
    vector<Tuple> getResult() const { return result; }
};

/*
define class NestedLoopJoin below
 */
class NestedLoopJoin : public JoinOperator {
public:
    NestedLoopJoin(vector<Tuple>& left, vector<Tuple>& right) : JoinOperator(left, right) {}
    void execute() {
        for (size_t i = 0; i < left.size(); i++)
            for (size_t j = 0; j < right.size(); j++)
                if (left[i].getKey() == right[j].getKey())
                    result.push_back(Tuple(left[i].getKey(), left[i].getValue() + right[j].getValue()));
    }
};


/*
 define class HashTable below
 */
class HashTable {
    vector<vector<HashEntry>> buckets;
    int hash(int key) {
        if (buckets.size() == 0) throw runtime_error("No entry");
        return key % buckets.size();
    }
public:
    HashTable(int size) : buckets(size) {}
    void insert(int key, int value) {
        HashEntry h(key, value);
        int idx = hash(key);
        buckets[idx].push_back(h);
    }
    vector<HashEntry>& getBucket(int key) {
        int idx = hash(key);
        if (idx < 0 || idx >= (int)buckets.size()) throw runtime_error("No entry");
        return buckets[idx];
    }
};


/*
 * Fill-in the class HashJoin below
 */
class HashJoin : public JoinOperator {
    /*define pointer ht for HashTable*/
    HashTable* ht;

    /*define populateHashTable() here*/
    void populateHashTable() {
        for (size_t i = 0; i < left.size(); i++) ht->insert(left[i].getKey(), left[i].getValue());
    }

    void FindMatch(Tuple& t,  vector<HashEntry> &bucket) {
        for (size_t i = 0; i < bucket.size(); i++)
            if (bucket[i].getKey() == t.getKey())
                result.push_back(Tuple(t.getKey(), bucket[i].getValue() + t.getValue()));
    }

public:
    /* constructor */
    HashJoin(vector<Tuple>& left, vector<Tuple>& right) : JoinOperator(left, right) {
        ht = new HashTable(left.size() + 1);
    }

    /* destructor*/
    ~HashJoin() { delete ht; }

    void execute() {
        populateHashTable();
        for (size_t i = 0; i < right.size(); i++) {
            vector<HashEntry>& bucket = ht->getBucket(right[i].getKey());
            FindMatch(right[i], bucket);
        }
    }
};


/* =========== DO NOT MODIFY THE CODE BELOW ============ READONLY CODE ==== */
void joinTest(vector<Tuple>& left, vector<Tuple>& right) {
    try {
        JoinOperator* join;

        cout << "Nested Loop Join:\n";
        join = new NestedLoopJoin(left, right);
        join->execute();
        vector<Tuple> nl = join->getResult();
        printTuples(nl);
        delete join;

        cout << "\nHash Join:\n";
        join = new HashJoin(left, right);
        join->execute();
        vector<Tuple> hj = join->getResult();
        printTuples(hj);
        delete join;

        if (nl.size() != hj.size()) {
            throw runtime_error("Result mismatch between impl");
        }

        for (int i = 0; i < nl.size(); i++) {
            vector<Tuple>::iterator where =
    std::find_if(hj.begin(), hj.end(),
        [&](const Tuple& t) {
            return t.getKey() == nl[i].getKey() &&
                   t.getValue() == nl[i].getValue();
        });
            if (where == hj.end()) {
                throw runtime_error("Result mismatch between impl");
            }
        }
    }
    catch (exception& e) {
        cout << "fail";
    }
}

int main() {

    while (true) {
        vector<Tuple> left;
        vector<Tuple> right;

        /* ========== END OF READONLY CODE =============== */

        // define variables as you need for cin
        int n, m, k, v;

        cout << "Enter number of tuples in LEFT table: ";
        cin >> n;

        cout << "Enter tuples for LEFT table (key value):\n";
        for (int i = 0; i < n; i++) { cin >> k >> v; left.push_back(Tuple(k, v)); }

        cout << "Enter number of tuples in RIGHT table: ";
        cin >> m;

        cout << "Enter tuples for RIGHT table (key value):\n";
        for (int i = 0; i < m; i++) { cin >> k >> v; right.push_back(Tuple(k, v)); }

        /* === READONLY ===== DO NOT MODIFY ====*/
        cout << "\nExecuting join...\n";
        joinTest(left, right);

        char choice;
        cout << "\nDo you want to run another test? (y/n): ";
        cin >> choice;

        if (choice == 'n' || choice == 'N') {
            cout << "Bye\n";
            break;
        }

    }
    return 0;
}
/* ==================== END OF READONLY CODE ====================*/
