#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 7. Conference: decideAcceptance, printing order ---" << endl;
    Conference c("ICPC");
    Reviewer* r = new Reviewer("Rita");
    c.addReviewer(r);
    ConferencePaper* p1 = new ConferencePaper("Good", "A");
    ConferencePaper* p2 = new ConferencePaper("Bad", "B");
    ConferencePaper* p3 = new ConferencePaper("NoReview", "C");
    ConferencePaper* p4 = new ConferencePaper("Draft", "D");
    ConferencePaper* p5 = new ConferencePaper("Edge", "E");
    c.addPaper(p1); c.addPaper(p2); c.addPaper(p3); c.addPaper(p4); c.addPaper(p5);
    for (int id : {1, 2, 3, 5}) { c.submitPaper(id); c.assignPaperToReviewer(id, "Rita"); }
    r->reviewPaper(9, 1); r->reviewPaper(8, 1);
    r->reviewPaper(6, 2);
    r->reviewPaper(7, 5);
    c.decideAcceptance();
    test(p1->getStatus() == "Accepted" && p2->getStatus() == "Rejected" && p5->getStatus() == "Accepted",
         "avg >= 7.0 -> Accepted (7.0 counts), else Rejected",
         "Statuses: " + p1->getStatus() + ", " + p2->getStatus() + ", " + p5->getStatus() + " expected Accepted, Rejected, Accepted");
    test(p3->getStatus() == "Under review" && p4->getStatus() == "In progress",
         "Papers with no reviews / not under review are left untouched",
         "Untouched papers changed: " + p3->getStatus() + ", " + p4->getStatus());
    string out = captureOut([&]() { c.display(); });
    string exp = "Rita\n"
                 "5 Edge: E 7.00 Accepted\n"
                 "4 Draft: D 0.00 In progress\n"
                 "3 NoReview: C 0.00 Under review\n"
                 "2 Bad: B 6.00 Rejected\n"
                 "1 Good: A 8.50 Accepted\n";
    test(out == exp,
         "display: reviewers (insertion order), then papers newest-first",
         "display printed\n" + out + "expected\n" + exp);
    return 0;
}
