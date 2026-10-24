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

bool logContains(const string& filename, const string& searchStr) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (line.find(searchStr) != string::npos) return true;
    }
    return false;
}

void printLogContents(const string& filename, const string& expectedOutput) {
    cout << "\n\t--- Log file contents ---\n";
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        cout << "\t" << line << "\n";
    }
    cout << "\n\t--- Expected Line Format ---\n";
    cout << "\t" << expectedOutput << "\n\n";
}

int main() {
    cout << "--- 2c. Testing Account sendMoney ---" << endl;
    remove("local_log.txt"); 
    Logger logger("local_log.txt");
    
    Account myAccount("1234567890", "User1", "user1@ok.iiitb");
    
    // Pre-fund the account so sendMoney has balance to work with
    myAccount.addMoney(500, logger);
    
    // Clear out the pre-funding logs to ensure a clean slate for sendMoney checks
    ofstream clearLog("local_log.txt", ios::trunc);
    clearLog.close();

    Account receiverAccount("0987654321", "Deepika", "deepika@ok.iiitb");
    myAccount.sendMoney(receiverAccount, 200, logger);
    
    test(myAccount.showBalance() == 300.0, 
        "Sender balance correct after sendMoney", 
        "Sender balance incorrect after sendMoney");

    bool sendMoneyLogOk = logContains("local_log.txt", "deepika@ok.iiitb");
    test(sendMoneyLogOk, 
        "sendMoney successfully wrote to logfile", 
        "sendMoney failed to write expected data to logfile");
        
    if (!sendMoneyLogOk) {
        cout << "Do not worry about trailling zeros" << endl;
        printLogContents("local_log.txt", "[user1@ok.iiitb]: Rs.200.000000 debited from user1@ok.iiitb");
    }

    return 0;
}