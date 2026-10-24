#include "lab_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- doAppAction / doUserAction wiring ---" << endl;
    remove("./upi_log.txt");
    UPISystem u;
    Account* cur = nullptr;
    runWithInput("9999999999\nTester\n", [&]() { doAppAction(u, cur, 1); });
    runWithInput("8888888888\nOther\n", [&]() { doAppAction(u, cur, 1); });
    bool noThrow = true;
    try { runWithInput("ghost\n", [&]() { doAppAction(u, cur, 2); }); } catch (...) { noThrow = false; }
    bool failOk = cur == nullptr && noThrow;
    runWithInput("Tester\n", [&]() { doAppAction(u, cur, 2); });
    test(failOk && cur && cur->getName() == "Tester", "REGISTER + LOGIN wired; a failed login leaves currentUser null and doesn't crash",
         "doAppAction REGISTER/LOGIN not wired correctly");

    runWithInput("700\n", [&]() { u.doUserAction(cur, 2); });
    runWithInput("8888888888\n200\n", [&]() { u.doUserAction(cur, 1); });
    runWithInput("nobody@ok.iiitb\n50\n", [&]() { u.doUserAction(cur, 1); });
    runWithInput("other@ok.iiitb\n100\n", [&]() { u.doUserAction(cur, 1); });
    double b = -1;
    string out = runWithInput("", [&]() { u.doUserAction(cur, 3); });
    b = cur->showBalance();
    test(near(b, 400) && normLine(out).find("Tester's balance: Rs.400") != string::npos,
         "ADD / SEND (by phone and by UPI ID) / CHECK_BALANCE wired", "Expected balance 400 after +700, -200, -100; got " + to_string(b));
    test(logHas("./upi_log.txt", "[tester@ok.iiitb]: Send money failed: [nobody@ok.iiitb] Receiver not registered on UPI."),
         "Unknown receiver: findAccount's exception logged as a send failure",
         "Expected log line: [tester@ok.iiitb]: Send money failed: [nobody@ok.iiitb] Receiver not registered on UPI.");

    bool ret = true;
    runWithInput("", [&]() { ret = u.doUserAction(cur, 4); });
    bool exitRet = true;
    Account* none = nullptr;
    runWithInput("", [&]() { exitRet = doAppAction(u, none, 3); });
    test(cur == nullptr && !ret && !exitRet,
         "LOGOUT sets currentUser to nullptr (needs Account*& parameter) and returns false; EXIT returns false",
         "After LOGOUT currentUser must be nullptr -- doUserAction must take Account*& currentUser");
    remove("./upi_log.txt");
    return 0;
}
