#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 4. Reviewer: assignPaper, reviewPaper rules ---" << endl;
    ConferencePaper p1("P1", "Alice");
    ConferencePaper p2("P2", "Bob");
    Reviewer r("Rita");
    r.assignPaper(&p1);
    r.assignPaper(&p1);   // duplicate -> ignored
    test(r.isAssigned(1) && !r.isAssigned(2) && r.getName() == "Rita",
         "assignPaper / isAssigned / getName work",
         "isAssigned wrong after assigning only paper 1");

    // score validity is checked FIRST, even for an unassigned paper
    int t1 = thrownInt([&]() { r.reviewPaper(11, 2); });
    int t2 = thrownInt([&]() { r.reviewPaper(0, 1); });
    test(t1 == 11 && t2 == 0,
         "Out-of-range score throws the score (checked before anything else)",
         "reviewPaper(11,2) should throw 11 and reviewPaper(0,1) throw 0; got " + to_string(t1) + ", " + to_string(t2));

    int t3 = thrownInt([&]() { r.reviewPaper(5, 2); });           // not assigned
    int t4 = thrownInt([&]() { r.reviewPaper(5, 1); });           // assigned, but status In progress
    test(t3 == 2 && t4 == 1 && p1.getReviewCount() == 0,
         "Unassigned paper / paper not Under review -> throws paperID, no score added",
         "Expected throws 2 and 1; got " + to_string(t3) + ", " + to_string(t4));

    p1.setStatus(PaperStatus::Under_Review);
    r.reviewPaper(9, 1);
    r.reviewPaper(6, 1);
    test(p1.getReviewCount() == 2 && p1.averageScore() == 7.5,
         "Valid reviews are recorded on the actual paper object",
         "After reviews 9 and 6, paper average should be 7.5, got " + to_string(p1.averageScore()));
    return 0;
}
