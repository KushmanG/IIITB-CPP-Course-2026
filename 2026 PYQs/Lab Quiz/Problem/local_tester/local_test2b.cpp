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
    cout << "--- 2b. Testing Account addMoney ---" << endl;
    remove("local_log.txt"); 
    Logger logger("local_log.txt");
    
    Account myAccount("1234567890", "User1", "user1@ok.iiitb");
    myAccount.addMoney(500, logger);
    
    test(myAccount.showBalance() == 500.0, 
        "Balance correctly reflects added money", 
        "Balance incorrect after addMoney");

    bool addMoneyLogOk = logContains("local_log.txt", "Added Rs.500");
    test(addMoneyLogOk, 
        "addMoney successfully wrote to logfile", 
        "addMoney failed to write expected data to logfile, check format.");
    
    if (!addMoneyLogOk) {
        cout << "Do not worry about trailling zeros" << endl;
        printLogContents("local_log.txt", "[user1@ok.iiitb]: Added Rs.500 to User1's wallet. Balance: Rs.500");
    }

    return 0;
}