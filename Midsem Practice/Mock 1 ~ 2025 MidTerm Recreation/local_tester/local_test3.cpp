#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 3. ConferencePaper: averageScore, display ---" << endl;
    ConferencePaper cp("NeuralODE", "Alice");
    test(cp.averageScore() == 0.0 && cp.getReviewCount() == 0,
         "No scores -> averageScore 0.0, review count 0",
         "Empty averageScore should be 0.0, got " + to_string(cp.averageScore()));
    cp.addReviewScore(7);
    cp.addReviewScore(8);
    test(cp.averageScore() == 7.5 && cp.getReviewCount() == 2,
         "averageScore is the mean of scores (7,8 -> 7.5)",
         "averageScore of 7,8 should be 7.5, got " + to_string(cp.averageScore()));
    cp.addAuthor("Bob");
    cp.setStatus(PaperStatus::Under_Review);
    string out = captureOut([&]() { cp.display(); });
    string exp = "1 NeuralODE: Alice Bob 7.50 Under review\n";
    test(out == exp,
         "display format correct",
         "display printed " + show(out) + ", expected " + show(exp));
    return 0;
}
