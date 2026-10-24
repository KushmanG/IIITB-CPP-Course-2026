#include <iostream>
#include <string>
#include <sstream>
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

bool logContains(const string& filename, const string& searchStr) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (line.find(searchStr) != string::npos) return true;
    }
    return false;
}

int main() {
    cout << "--- 4. Testing Actions ---" << endl;
    remove("./upi_log.txt"); // Clear old logs
    UPISystem upiSystem;
    Account *currentUser = nullptr;

    streambuf* origCin = cin.rdbuf();

    // 1. Test doAppAction: REGISTER
    stringstream mockReg("9999999999\nTestUser\n");
    cin.rdbuf(mockReg.rdbuf());
    doAppAction(upiSystem, currentUser, static_cast<int>(MainMenuCmd::REGISTER));
    
    test(logContains("./upi_log.txt", "9999999999"), 
        "doAppAction REGISTER logged correctly", 
        "doAppAction REGISTER missing in log");

    // 2. Test doAppAction: LOGIN
    stringstream mockLogin("9999999999\n");
    cin.rdbuf(mockLogin.rdbuf());
    doAppAction(upiSystem, currentUser, static_cast<int>(MainMenuCmd::LOGIN));

    // 3. Test doUserAction: ADD_MONEY
    if (currentUser) {
        stringstream mockAdd("5000\n");
        cin.rdbuf(mockAdd.rdbuf());
        upiSystem.doUserAction(currentUser, static_cast<int>(UserMenuCmd::ADD_MONEY));
        
        test(currentUser->showBalance() == 5000.0, 
            "doUserAction ADD_MONEY updated balance", 
            "doUserAction ADD_MONEY failed to update balance");

        // Verify the logger caught the action from the menu
        test(logContains("./upi_log.txt", "5000"), 
            "doUserAction ADD_MONEY logged correctly", 
            "doUserAction ADD_MONEY missing in log");
    } else {
        test(false, "", "Cannot test ADD_MONEY because Login failed");
    }

    cin.rdbuf(origCin);
    return 0;
}