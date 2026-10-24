#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 8. Exchange ---" << endl;
    remove("t8_log.txt");
    Exchange ex("t8_log.txt");
    ex.registerTrader("alice", 1000);
    ex.registerTrader("bob", 500);
    string d1 = thrownWhat([&]() { ex.registerTrader("alice", 5); });
    string d2 = thrownWhat([&]() { ex.registerTrader("zed", -1); });
    test(d1 == "Trader already registered" && d2 == "Invalid cash",
         "registerTrader rejects duplicates and negative cash (throws)",
         "Got " + d1 + " / " + d2);

    int r1 = ex.placeOrder("dave", "TCS", Side::BUY, 10, 1);
    int r2 = ex.placeOrder("alice", "TCS", Side::BUY, 60, 20);     // 1200 > 1000
    int r3 = ex.placeOrder("bob", "TCS", Side::SELL, 50, 10);      // first real order -> id 1
    test(r1 == -1 && r2 == -1 && r3 == 1,
         "placeOrder returns -1 on rejection; rejected orders consume no ID",
         "Expected -1, -1, 1; got " + to_string(r1) + ", " + to_string(r2) + ", " + to_string(r3));

    int r4 = ex.placeOrder("alice", "TCS", Side::BUY, 55, 4);      // trades 4@50
    test(r4 == 2 && ex.getPosition("alice", "TCS") == 4 && ex.getPosition("bob", "TCS") == -4 &&
         near(ex.getCash("alice"), 800) && near(ex.getCash("bob"), 700) && ex.getPosition("alice", "INFY") == 0 &&
         thrownWhat([&]() { ex.getCash("zed"); }) == "Unknown trader zed",
         "Settlement: buyer +qty / -value, seller -qty / +value at the trade price",
         "Positions/cash wrong after 4@50.00: alice " + to_string(ex.getPosition("alice", "TCS")) + " " + fmt2(ex.getCash("alice")));

    bool k1 = ex.cancelOrder("alice", "TCS", 1);
    bool k2 = ex.cancelOrder("bob", "TCS", 1);
    test(!k1 && k2 && thrownWhat([&]() { ex.getBook("NIFTY"); }) == "Unknown symbol NIFTY" && ex.getBook("TCS").restingCount() == 0,
         "cancelOrder returns false/true; getBook throws for unknown symbol",
         "cancelOrder / getBook wrong");

    string exp = "REGISTER alice 1000.00\nREGISTER bob 500.00\n"
                 "REJECT dave: Unknown trader dave\nREJECT alice: Insufficient cash\n"
                 "ACCEPT #1 SELL bob TCS 10/10@50.00\nACCEPT #2 BUY alice TCS 4/4@55.00\n"
                 "TRADE TCS 4@50.00 B#2:alice S#1:bob\n"
                 "REJECT alice: Not your order\nCANCEL #1 bob\n";
    string got = readFile("t8_log.txt");
    test(got == exp && ex.getLogCount() == 9,
         "Log file contents exact",
         "Log file was\n" + got + "expected\n" + exp);
    remove("t8_log.txt");
    return 0;
}
