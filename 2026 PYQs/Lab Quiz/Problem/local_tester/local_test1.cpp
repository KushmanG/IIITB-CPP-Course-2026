#include <iostream>
#include <string>

#define main app_original_main
#include "student.cpp"
#undef main

using namespace std;

void test(bool condition, const string& success, const string& failure) {
    if (condition) cout << "[PASS] " << success << endl;
    else cout << "[FAIL] " << failure << endl;
}

int main() {
    cout << "--- 1. Testing Wallet ---" << endl;
    Wallet myWallet;
    myWallet += 200;
    myWallet -= 100;
    
    test(myWallet.getBalance() == 100.0, 
        "Wallet balance correctly updated", 
        "Wallet balance is incorrect got " + to_string(myWallet.getBalance()) + ", expected 100.0");

    return 0;
}