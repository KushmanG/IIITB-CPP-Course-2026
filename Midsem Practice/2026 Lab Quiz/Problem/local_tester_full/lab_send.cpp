#include "lab_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

static double bal(Account& a) { double b = 0; captureOut([&]() { b = a.showBalance(); }); return b; }
int main() {
    cout << "--- Account::sendMoney ---" << endl;
    remove("t_log.txt");
    { Logger lg("t_log.txt");
      Account s("1", "User1", "user1@ok.iiitb"), r("2", "Deepika", "deepika@ok.iiitb");
      s.addMoney(500, lg);
      remove("t_log.txt");
      Logger lg2("t_log.txt");
      s.sendMoney(r, 200, lg2);
      test(near(bal(s), 300) && near(bal(r), 200), "Sender debited AND receiver credited", "Balances after sending 200: sender " + to_string(bal(s)) + ", receiver " + to_string(bal(r)));
      testLog("t_log.txt", {"[user1@ok.iiitb]: Rs.200 debited from user1@ok.iiitb",
                            "[deepika@ok.iiitb]: Added Rs.200 to Deepika's wallet. Balance: Rs.200"},
              "Success: debit line, then the receiver's addMoney line");
      remove("t_log.txt"); }
    { Logger lg("t_log.txt");
      Account s("1", "User1", "user1@ok.iiitb"), r("2", "Deepika", "deepika@ok.iiitb");
      s.addMoney(20000, lg);
      remove("t_log.txt");
      Logger lg2("t_log.txt");
      bool escaped = false;
      try { s.sendMoney(r, 10000.5, lg2); s.sendMoney(s, 100, lg2); s.sendMoney(r, 10000, lg2); } catch (...) { escaped = true; }
      testLog("t_log.txt", {"[user1@ok.iiitb]: Send money failed: Transaction limit exceeded (max Rs.10000).",
                            "[user1@ok.iiitb]: Send money failed: Cannot send money to self.",
                            "[user1@ok.iiitb]: Rs.10000 debited from user1@ok.iiitb",
                            "[deepika@ok.iiitb]: Added Rs.10000 to Deepika's wallet. Balance: Rs.10000"},
              "Over limit and self-transfer rejected; exactly 10000 allowed");
      test(!escaped && near(bal(s), 10000) && near(bal(r), 10000), "Rejected sends change nothing and are caught inside sendMoney", "Balances wrong or exception escaped");
      remove("t_log.txt"); }
    { Logger lg("t_log.txt");
      Account s("1", "User1", "user1@ok.iiitb"), r("2", "Deepika", "deepika@ok.iiitb");
      s.addMoney(50, lg);
      remove("t_log.txt");
      Logger lg2("t_log.txt");
      try { s.sendMoney(r, 80, lg2); } catch (...) {}
      testLog("t_log.txt", {"[user1@ok.iiitb]: Send money failed: Balance not sufficient!"}, "Insufficient balance: wallet's exception logged, receiver not credited");
      test(near(bal(s), 50) && near(bal(r), 0), "Insufficient balance leaves both balances unchanged", "Balances changed on a failed send");
      remove("t_log.txt"); }
    return 0;
}
