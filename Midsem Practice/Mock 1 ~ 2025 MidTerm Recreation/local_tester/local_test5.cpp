#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 5. Conference: add/find, submitPaper ---" << endl;
    Conference c("ICPC");
    Reviewer* r1 = new Reviewer("Rita");
    Reviewer* r2 = new Reviewer("Rita");
    c.addReviewer(r1);
    string w = thrownInvalidArg([&]() { c.addReviewer(r2); });
    test(w == "Rita",
         "Duplicate reviewer name throws invalid_argument(name)",
         "addReviewer duplicate: expected invalid_argument with what()=\"Rita\", got " + w);
    test(c.findReviewer("Rita") == r1 && thrownInvalidArg([&]() { c.findReviewer("Zed"); }) == "Zed",
         "findReviewer returns the pointer / throws invalid_argument(name)",
         "findReviewer wrong");

    ConferencePaper* p = new ConferencePaper("Sorting", "Alice");
    c.addPaper(p);
    c.addPaper(p);   // duplicate -> ignored
    string out = captureOut([&]() { c.printAllPapers(); });
    test(c.findPaper(1) == p && thrownInt([&]() { c.findPaper(42); }) == 42 && out == "1 Sorting: Alice 0.00 In progress\n",
         "findPaper works, throws the id if missing, duplicate addPaper ignored",
         "findPaper / addPaper wrong; printAllPapers gave " + show(out));

    c.submitPaper(1);
    int t = thrownInt([&]() { c.submitPaper(1); });
    int t2 = thrownInt([&]() { c.submitPaper(7); });
    test(p->getStatus() == "Submitted" && t == 1 && t2 == 7,
         "submitPaper: In progress -> Submitted; resubmit / missing id throw the id",
         "submitPaper wrong: status " + p->getStatus() + ", throws " + to_string(t) + ", " + to_string(t2));
    return 0;
}
