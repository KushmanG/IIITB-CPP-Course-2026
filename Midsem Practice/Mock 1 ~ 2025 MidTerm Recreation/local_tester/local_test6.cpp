#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 6. Conference: assignPaperToReviewer ---" << endl;
    Conference c("ICPC");
    Reviewer* ra = new Reviewer("Rita");
    Reviewer* rb = new Reviewer("Sam");
    Reviewer* rc = new Reviewer("Alice");
    c.addReviewer(ra); c.addReviewer(rb); c.addReviewer(rc);
    ConferencePaper* p1 = new ConferencePaper("P1", "Alice");
    ConferencePaper* p2 = new ConferencePaper("P2", "Bob");
    c.addPaper(p1); c.addPaper(p2);

    int t1 = thrownInt([&]() { c.assignPaperToReviewer(9, "Nobody"); });
    string t2 = thrownInvalidArg([&]() { c.assignPaperToReviewer(1, "Nobody"); });
    test(t1 == 9 && t2 == "Nobody",
         "Missing paper checked before missing reviewer (int id, then invalid_argument)",
         "assign(9,Nobody) should throw 9; assign(1,Nobody) invalid_argument(Nobody); got " + to_string(t1) + ", " + t2);

    int t3 = thrownInt([&]() { c.assignPaperToReviewer(1, "Rita"); });   // still In progress
    c.submitPaper(1);
    int t4 = thrownInt([&]() { c.assignPaperToReviewer(1, "Alice"); });  // author reviewing own paper
    test(t3 == 1 && t4 == 1 && !rc->isAssigned(1),
         "Not-submitted paper and author-as-reviewer both throw paperID",
         "Expected 1 and 1, got " + to_string(t3) + ", " + to_string(t4));

    c.assignPaperToReviewer(1, "Rita");
    test(ra->isAssigned(1) && !rb->isAssigned(1) && p1->getStatus() == "Under review",
         "Paper assigned ONLY to the named reviewer; status -> Under review",
         "After assign(1,Rita): Rita " + string(ra->isAssigned(1) ? "has" : "lacks") + " it, Sam " + string(rb->isAssigned(1) ? "has" : "lacks") + " it, status " + p1->getStatus());

    c.assignPaperToReviewer(1, "Sam");   // Under review is still assignable
    test(rb->isAssigned(1),
         "A paper already Under review can get more reviewers",
         "assign(1,Sam) on an Under-review paper failed");
    return 0;
}
