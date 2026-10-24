#include <iostream>
#include <vector>
using namespace std;


/* define class Tuple below*/


/* define class HashEntry below*/



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

/*
define class NestedLoopJoin below
 */


/*
 define class HashTable below
 */


/*
 * Fill-in the class HashJoin below
 */
class HashJoin : /* inherit JoinOperator*/ {
    /*define pointer ht for HashTable*/

    /*define populateHashTable() here*/

    void FindMatch(Tuple& t,  vector<HashEntry> &bucket) {
        /*fill-in the logic*/
    }

public:
    /* constructor */

    /* destructor*/

    void execute() {
         /* call populateHashTable() method here*/

        /* do for loop for right table*/
        /* for each tuple t in right
         *     bucket = ht->getBucket(t.getKey())
         *     call FindMatch method with parameter t and bucket -- this will do the join inside FindMatch
         */
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

        cout << "Enter number of tuples in LEFT table: ";
        // do proper cin

        cout << "Enter tuples for LEFT table (key value):\n";
        // run a for loop to take cin, make a tuple, put in left vector

        cout << "Enter number of tuples in RIGHT table: ";
        // do proper cin

        cout << "Enter tuples for RIGHT table (key value):\n";
        // run a for loop to take cin, make a tuple, put in right vector

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


