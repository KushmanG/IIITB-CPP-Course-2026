#include "lab_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- Account basics, showBalance ---" << endl;
    Account acc("1234567890", "Aryan", "aryan@ok.iiitb");
    test(acc.getPhone() == "1234567890" && acc.getName() == "Aryan" && acc.getUpiId() == "aryan@ok.iiitb", "Constructor and getters", "Account getters wrong");
    double r = -1;
    string out = captureOut([&]() { r = acc.showBalance(); });
    test(near(r, 0) && normLine(out) == normLine("Aryan's balance: Rs.0"), "showBalance prints \"<name>'s balance: Rs.<balance>\" and RETURNS the balance",
         "showBalance printed " + show(out) + " and returned " + to_string(r));
    return 0;
}
