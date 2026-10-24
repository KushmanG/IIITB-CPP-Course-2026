#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 2. Order, BuyOrder, SellOrder ---" << endl;
    BuyOrder b("alice", "INFY", 100, 10);
    string w1 = thrownWhat([]() { SellOrder s("bob", "INFY", 100, 0); });
    string w2 = thrownWhat([]() { SellOrder s("bob", "INFY", -5, 3); });
    SellOrder s("bob", "INFY", 100, 8);
    test(b.getId() == 1 && w1 == "Invalid quantity" && w2 == "Invalid price" && s.getId() == 2,
         "IDs from 1; invalid quantity/price throw and do NOT consume an ID",
         "Expected id 1, \"Invalid quantity\", \"Invalid price\", id 2; got " + to_string(b.getId()) + ", " + w1 + ", " + w2 + ", " + to_string(s.getId()));

    test(b.describe() == "#1 BUY alice INFY 10/10@100.00" && s.describe() == "#2 SELL bob INFY 8/8@100.00",
         "describe() format",
         "describe() gave " + show(b.describe()) + " and " + show(s.describe()));

    b -= 4;
    string w3 = thrownWhat([&]() { b -= 7; });
    string w4 = thrownWhat([&]() { b -= 0; });
    b -= 6;
    test(w3 == "Invalid fill" && w4 == "Invalid fill" && b.getRemaining() == 0 && b.isFilled() && b.getQuantity() == 10,
         "operator-= fills, rejects q<=0 or q>remaining with \"Invalid fill\"",
         "operator-= wrong: remaining " + to_string(b.getRemaining()) + ", errors " + w3 + " / " + w4);

    BuyOrder bb("x", "INFY", 100, 1);
    SellOrder ss("y", "INFY", 100, 1);
    Order& r = ss;
    test(bb.crosses(100) && bb.crosses(99.5) && !bb.crosses(100.5) &&
         ss.crosses(100) && ss.crosses(100.5) && !ss.crosses(99.5) && r.side() == Side::SELL,
         "crosses(): buy >= other, sell <= other; side() is virtual",
         "crosses()/side() wrong");
    return 0;
}
