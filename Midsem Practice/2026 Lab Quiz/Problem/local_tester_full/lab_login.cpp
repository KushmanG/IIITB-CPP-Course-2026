#include "lab_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- UPISystem::login ---" << endl;
    remove("./upi_log.txt");
    UPISystem u;
    try { u.registerUser("1111111111", "Aryan"); u.registerUser("2222222222", "Bob"); } catch (...) {}
    { ofstream clear("./upi_log.txt", ios::trunc); }   // empty the log (the Logger keeps appending to it)
    Account* a = nullptr; Account* b = nullptr;
    try { a = u.login("1111111111"); b = u.login("Bob"); } catch (...) {}
    test(a && a->getName() == "Aryan" && b && b->getPhone() == "2222222222", "Login by phone and by exact name", "login returned the wrong account / nullptr");
    Account* c = (Account*)1; bool threwUpi = false, threwOther = false;
    try { c = u.login("aryan"); } catch (UPIException&) { threwUpi = true; c = nullptr; } catch (...) { threwOther = true; }
    test((c == nullptr || threwUpi) && !threwOther, "Unknown identifier (name is case-sensitive): nullptr or UPIException", "login(\"aryan\") should fail (exact name match)");
    testLog("./upi_log.txt", {"[aryan@ok.iiitb]: Login successful.", "[bob@ok.iiitb]: Login successful.",
                              "[UPISystem]: Login failed: No such user aryan. Check phone number or name."},
            "Login success/failure log lines (logged by login or its caller)");
    remove("./upi_log.txt");
    return 0;
}
