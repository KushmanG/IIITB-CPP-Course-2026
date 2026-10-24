#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 2. Paper: virtual averageScore, addAuthor exception, displayPaper ---" << endl;
    Paper base("Base");
    ConferencePaper cp("Topology", "Alice");
    cp.addReviewScore(8);
    Paper& ref = cp;
    test(base.averageScore() == -1.0 && ref.averageScore() == 8.0,
         "averageScore is virtual (base -1.0, derived via Paper& = 8.0)",
         "averageScore: base " + to_string(base.averageScore()) + ", via Paper& " + to_string(ref.averageScore()) + " (is it virtual?)");
    cp.setStatus(PaperStatus::Submitted);
    int t = thrownInt([&]() { cp.addAuthor("Bob"); });
    test(t == cp.getID() && cp.getAuthors().size() == 1,
         "addAuthor on a non-In-progress paper throws its paperID (int), author not added",
         "addAuthor on submitted paper should throw int " + to_string(cp.getID()));
    string out = captureOut([&]() { cp.displayPaper(); });
    string exp = "2 Topology: Alice ";
    test(out == exp,
         "displayPaper format correct",
         "displayPaper printed " + show(out) + ", expected " + show(exp));
    return 0;
}
