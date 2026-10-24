#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 7. OrderBook: cancel, best prices, depth ---" << endl;
    OrderBook book("INFY");
    string e1 = thrownWhat([&]() { book.bestBid(); });
    string e2 = thrownWhat([&]() { book.bestAsk(); });
    book.submit(make_shared<SellOrder>("bob", "INFY", 101, 5));   // 1
    string e3 = thrownWhat([&]() { book.spread(); });
    test(e1 == "No bids" && e2 == "No asks" && e3 == "No bids",
         "Empty side throws \"No bids\"/\"No asks\"; spread checks bids first",
         "Got " + e1 + " / " + e2 + " / " + e3);

    string c1 = thrownWhat([&]() { book.cancel(9, "bob"); });
    string c2 = thrownWhat([&]() { book.cancel(1, "eve"); });
    bool still = book.find(1) != nullptr;
    book.cancel(1, "bob");
    test(c1 == "Order not found" && c2 == "Not your order" && still && book.find(1) == nullptr,
         "cancel: missing id / other trader throw; owner removes it",
         "cancel errors were " + c1 + " / " + c2);

    book.submit(make_shared<SellOrder>("bob", "INFY", 102, 5));    // 2
    book.submit(make_shared<SellOrder>("carol", "INFY", 101, 3));  // 3
    book.submit(make_shared<SellOrder>("dan", "INFY", 102, 7));    // 4
    book.submit(make_shared<BuyOrder>("eve", "INFY", 99, 4));      // 5
    book.submit(make_shared<BuyOrder>("fay", "INFY", 100, 6));     // 6
    book.submit(make_shared<BuyOrder>("gus", "INFY", 99, 1));      // 7
    string out = captureOut([&]() { book.printDepth(); });
    string exp = "ASK 102.00 12 (2)\nASK 101.00 3 (1)\n---\nBID 100.00 6 (1)\nBID 99.00 5 (2)\n";
    test(out == exp,
         "printDepth aggregates by price; both sides highest price first",
         "printDepth printed\n" + out + "expected\n" + exp);

    book.submit(make_shared<BuyOrder>("hal", "INFY", 101, 2));     // 8: partially fills #3
    string out2 = captureOut([&]() { book.printDepth(); });
    test(out2.find("ASK 101.00 1 (1)") != string::npos,
         "printDepth uses REMAINING quantity",
         "After a partial fill, depth should show ASK 101.00 1 (1); got\n" + out2);
    return 0;
}
