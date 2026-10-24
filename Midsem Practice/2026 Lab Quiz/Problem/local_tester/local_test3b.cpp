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
    cout << "--- 3b. Testing UPISystem login ---" << endl;
    
    UPISystem upiSystem;
    
    // Pre-register user to populate system state before testing login
    try { upiSystem.registerUser("1111111111", "User2"); } catch (...) {}
    
    // Clear logs from registration so we only verify login logs
    remove("./upi_log.txt"); 

    Account *loggedInAccount = upiSystem.login("1111111111");
    test(loggedInAccount != nullptr, 
        "Login successful (returned valid Account pointer)", 
        "Login failed (returned nullptr)");

   

    return 0;
}