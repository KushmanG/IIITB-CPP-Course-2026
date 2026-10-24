#include "lab_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- Wallet ---" << endl;
    Wallet w;
    (w += 200) -= 50;
    test(near(w.getBalance(), 150), "New wallet is 0; += and -= update and chain", "Expected 150 after +=200, -=50; got " + to_string(w.getBalance()));
    string a = thrownWhat([&]() { w += 0; });
    string b = thrownWhat([&]() { w -= -5; });
    bool isUpi = false; try { w += -1; } catch (UPIException&) { isUpi = true; } catch (...) {}
    test(a == "Invalid amount." && b == "Invalid amount." && isUpi && near(w.getBalance(), 150),
         "Non-positive credit/debit throws UPIException(\"Invalid amount.\")", "Got " + a + " / " + b);
    string c = thrownWhat([&]() { w -= 150.01; });
    w -= 150;
    test(c == "Balance not sufficient!" && near(w.getBalance(), 0), "Debit above balance throws \"Balance not sufficient!\"; exact balance allowed", "Got " + c);
    return 0;
}
