#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 1. ExchangeException and Logger ---" << endl;
    string w = thrownWhat([]() { throw ExchangeException("boom"); });
    test(w == "boom",
         "ExchangeException is a runtime_error carrying its message",
         "throw ExchangeException(\"boom\") caught as runtime_error& gave what() = " + w);

    remove("t1_log.txt");
    {
        Logger lg("t1_log.txt");
        lg.log("first");
        lg.log("second");
        test(lg.getCount() == 2 && readFile("t1_log.txt") == "first\nsecond\n",
             "Logger writes one line per log() and counts them (flushed immediately)",
             "Log file after 2 logs: " + show(readFile("t1_log.txt")) + ", count " + to_string(lg.getCount()));
    }
    {
        Logger lg2("t1_log.txt");
        lg2.log("third");
        test(readFile("t1_log.txt") == "first\nsecond\nthird\n" && lg2.getCount() == 1,
             "A new Logger on the same file APPENDS (count starts at 0)",
             "Second Logger should append; file is " + show(readFile("t1_log.txt")));
    }
    remove("t1_log.txt");
    return 0;
}
