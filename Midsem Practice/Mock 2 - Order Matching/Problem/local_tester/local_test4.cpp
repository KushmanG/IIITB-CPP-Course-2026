#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 4. OrderBook: basic matching ---" << endl;
    OrderBook book("INFY");
    string w = thrownWhat([&]() { book.submit(make_shared<BuyOrder>("a", "TCS", 10, 1)); });   // id 1
    test(w == "Wrong symbol" && book.restingCount() == 0,
         "Order for another symbol throws \"Wrong symbol\"",
         "Expected \"Wrong symbol\", got " + w);

    auto ask = make_shared<SellOrder>("bob", "INFY", 100, 10);    // id 2
    vector<Trade> t0 = book.submit(ask);
    auto buy = make_shared<BuyOrder>("alice", "INFY", 101, 4);    // id 3
    vector<Trade> t1 = book.submit(buy);
    test(t0.empty() && t1.size() == 1 && t1[0].getQuantity() == 4 && near(t1[0].getPrice(), 100) &&
         t1[0].getBuyId() == 3 && t1[0].getSellId() == 2 && book.find(3) == nullptr && ask->getRemaining() == 6,
         "Crossing order trades at the RESTING order's price; filled incoming order does not rest",
         "Expected one trade 4@100.00 B#3 S#2, ask remaining 6");

    auto buy2 = make_shared<BuyOrder>("carol", "INFY", 100, 10);  // id 4
    vector<Trade> t2 = book.submit(buy2);
    test(t2.size() == 1 && t2[0].getQuantity() == 6 && book.find(2) == nullptr &&
         book.find(4) == buy2 && buy2->getRemaining() == 4 && near(book.bestBid(), 100) && book.restingCount() == 1,
         "Partial fill: filled resting order removed, remainder of incoming rests",
         "Expected trade of 6, ask #2 gone, bid #4 resting with 4 left");
    return 0;
}
