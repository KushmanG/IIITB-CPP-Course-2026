#include "tester_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- 3. Trade ---" << endl;
    Trade t("INFY", 4, "alice", 2, "carol", 100, 30);
    ostringstream os; os << t;
    string exp = "TRADE INFY 30@100.00 B#4:alice S#2:carol";
    test(os.str() == exp,
         "operator<< format",
         "operator<< printed " + show(os.str()) + ", expected " + show(exp));
    test(t.getBuyId() == 4 && t.getSellId() == 2 && t.getBuyer() == "alice" && t.getSeller() == "carol" &&
         near(t.getPrice(), 100) && t.getQuantity() == 30 && t.getSymbol() == "INFY",
         "Trade getters",
         "Trade getters return wrong values");
    return 0;
}
