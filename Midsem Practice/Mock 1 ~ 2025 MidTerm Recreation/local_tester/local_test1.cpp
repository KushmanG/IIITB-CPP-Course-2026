#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 1. Paper: constructors, IDs, authors ---" << endl;
    Paper p1("Quantum");
    Paper p2("Graphs", "Alice");
    test(p1.getID() == 1 && p2.getID() == 2,
         "IDs auto-assigned from 1 in creation order",
         "IDs wrong: got " + to_string(p1.getID()) + ", " + to_string(p2.getID()) + " expected 1, 2");
    test(p1.getStatus() == "In progress" && p2.getStatus() == "In progress",
         "New paper status is \"In progress\"",
         "New paper status wrong: got \"" + p2.getStatus() + "\"");
    p2.addAuthor("Bob");
    p2.addAuthor("Alice");   // duplicate -> ignored
    vector<string> a = p2.getAuthors();
    test(a.size() == 2 && a[0] == "Alice" && a[1] == "Bob",
         "addAuthor appends in order and ignores duplicates",
         "Authors wrong after addAuthor(Bob), addAuthor(Alice): size " + to_string(a.size()));
    test(p2.hasAuthor("Bob") && !p2.hasAuthor("Carol"),
         "hasAuthor works",
         "hasAuthor returned wrong value");
    return 0;
}
