#include "lab_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- Account::addMoney ---" << endl;
    remove("t_log.txt");
    { Logger lg("t_log.txt");
      Account acc("1", "User1", "user1@ok.iiitb");
      acc.addMoney(1000, lg);
      testLog("t_log.txt", {"[user1@ok.iiitb]: Added Rs.1000 to User1's wallet. Balance: Rs.1000"}, "Success log line exact");
      remove("t_log.txt"); }
    { Logger lg("t_log.txt");
      Account acc("1", "User1", "user1@ok.iiitb");
      bool escaped = false;
      try { acc.addMoney(20000, lg); acc.addMoney(20000.5, lg); } catch (...) { escaped = true; }
      testLog("t_log.txt", {"[user1@ok.iiitb]: Added Rs.20000 to User1's wallet. Balance: Rs.20000",
                            "[user1@ok.iiitb]: Add-money failed: Add-money limit exceeded (max Rs.20000)."},
              "Limit: 20000 allowed, above it logged as failure");
      double bal = 0; captureOut([&]() { bal = acc.showBalance(); });
      test(!escaped && near(bal, 20000), "Failure is caught INSIDE addMoney; balance unchanged", "addMoney let an exception escape or changed the balance");
      remove("t_log.txt"); }
    { Logger lg("t_log.txt");
      Account acc("1", "User1", "user1@ok.iiitb");
      try { acc.addMoney(-5, lg); } catch (...) {}
      testLog("t_log.txt", {"[user1@ok.iiitb]: Add-money failed: Invalid amount."}, "Wallet's exception is caught and logged by addMoney");
      remove("t_log.txt"); }
    return 0;
}
