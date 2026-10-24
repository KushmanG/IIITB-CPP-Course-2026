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
    cout << "--- 3a. Testing UPISystem registerUser ---" << endl;
    remove("./upi_log.txt"); 
    UPISystem upiSystem;

    upiSystem.registerUser("1111111111", "User2");
    
    bool regLogOk = logContains("./upi_log.txt", "1111111111");
    test(regLogOk, 
        "registerUser successfully wrote to upi_log.txt", 
        "registerUser failed to write to upi_log.txt");
        
    if (!regLogOk) {
        printLogContents("./upi_log.txt", "[user2@ok.iiitb]: Registered User2 | Phone: 1111111111 | UPI ID: user2@ok.iiitb");
    }

    return 0;
}