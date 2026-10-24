#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>

#define main app_original_main
#include "student.cpp"
#undef main

using namespace std;

void test(bool condition, const string& success, const string& failure) {
    if (condition) cout << "[PASS] " << success << endl;
    else cout << "[FAIL] " << failure << endl;
}

int main() {
    cout << "--- 2a. Testing Account Constructor ---" << endl;
    
    Account myAccount("1234567890", "User1", "user1@ok.iiitb");
    
    test(myAccount.getPhone() == "1234567890" && myAccount.getName() == "User1" && myAccount.getUpiId() == "user1@ok.iiitb", 
        "Account object initialized with correct phone, name, and UPI ID", 
        "Account object NOT initialized with correct properties");

    return 0;
}