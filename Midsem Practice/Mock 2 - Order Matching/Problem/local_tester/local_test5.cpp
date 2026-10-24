#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

static string seq(const vector<Trade>& v) {
    string s;
    for (const Trade& t : v) s += to_string(t.getQuantity()) + "@" + fmt2(t.getPrice()) + "/" + to_string(t.getBuyId()) + "-" + to_string(t.getSellId()) + " ";
    return s;
}
int main() {
    cout << "--- 5. OrderBook: price-time priority ---" << endl;
    OrderBook book("INFY");
    book.submit(make_shared<SellOrder>("bob", "INFY", 101, 50));    // 1
    book.submit(make_shared<SellOrder>("carol", "INFY", 100, 30));  // 2
    book.submit(make_shared<SellOrder>("bob", "INFY", 100, 40));    // 3
    vector<Trade> t = book.submit(make_shared<BuyOrder>("alice", "INFY", 101, 100));  // 4
    string exp = "30@100.00/4-2 40@100.00/4-3 30@101.00/4-1 ";
    test(seq(t) == exp,
         "Buy sweeps asks lowest price first, earliest ID first at equal price",
         "Trades were " + seq(t) + " expected " + exp);

    book.submit(make_shared<BuyOrder>("alice", "INFY", 99, 10));    // 5
    book.submit(make_shared<BuyOrder>("carol", "INFY", 99.5, 5));   // 6
    book.submit(make_shared<BuyOrder>("dan", "INFY", 99.5, 5));     // 7
    vector<Trade> t2 = book.submit(make_shared<SellOrder>("bob", "INFY", 99, 12));   // 8
    string exp2 = "5@99.50/6-8 5@99.50/7-8 2@99.00/5-8 ";
    test(seq(t2) == exp2,
         "Sell hits bids highest price first, earliest ID first at equal price",
         "Trades were " + seq(t2) + " expected " + exp2);

    vector<Trade> t3 = book.submit(make_shared<BuyOrder>("eve", "INFY", 100.5, 5));  // 9 (best ask 101)
    test(t3.empty() && near(book.bestBid(), 100.5) && near(book.bestAsk(), 101) && near(book.spread(), 0.5),
         "Non-crossing order rests; bestBid/bestAsk/spread",
         "Expected no trade, bestBid 100.5, bestAsk 101, spread 0.5");
    return 0;
}
