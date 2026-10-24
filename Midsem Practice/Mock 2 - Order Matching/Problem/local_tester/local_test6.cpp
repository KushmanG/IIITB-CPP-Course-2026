#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 6. OrderBook: self-trade prevention ---" << endl;
    OrderBook book("TCS");
    auto a1 = make_shared<SellOrder>("alice", "TCS", 40, 10);   // 1
    auto b2 = make_shared<SellOrder>("bob", "TCS", 45, 10);     // 2
    book.submit(a1); book.submit(b2);
    vector<Trade> t = book.submit(make_shared<BuyOrder>("alice", "TCS", 45, 10));  // 3
    test(t.size() == 1 && t[0].getSellId() == 2 && near(t[0].getPrice(), 45) && a1->getRemaining() == 10 && book.find(1) == a1,
         "Own resting order is skipped (even if better priced); next eligible order is used",
         "Expected one trade with #2 at 45.00 and alice's #1 untouched");

    auto own = make_shared<BuyOrder>("alice", "TCS", 50, 5);    // 4
    vector<Trade> t2 = book.submit(own);
    test(t2.empty() && book.find(4) == own && book.restingCount() == 2,
         "If only own orders cross, nothing trades and the order rests (book may stay crossed)",
         "Expected no trades and order #4 resting");
    return 0;
}
