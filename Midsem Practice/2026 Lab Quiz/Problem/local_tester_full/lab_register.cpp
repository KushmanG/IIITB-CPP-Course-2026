#include "lab_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

int main() {
    cout << "--- UPISystem::registerUser ---" << endl;
    remove("./upi_log.txt");
    { UPISystem u;
      try { u.registerUser("1234567890", "Aryan"); } catch (...) {}
      testLog("./upi_log.txt", {"[UPISystem]: Registered Aryan | Phone: 1234567890 | UPI ID: aryan@ok.iiitb"}, "Success log line exact (UPI ID generated)");
    }
    remove("./upi_log.txt");
    { UPISystem u;
      try { u.registerUser("1234567890", "Aryan"); } catch (...) {}
      try { u.registerUser("1234567890", "Bob"); } catch (...) {}
      try { u.registerUser("5555555555", "ARYAN"); } catch (...) {}
      try { u.registerUser("1234567890", "aryan"); } catch (...) {}
      testLog("./upi_log.txt", {"[UPISystem]: Registered Aryan | Phone: 1234567890 | UPI ID: aryan@ok.iiitb",
                                "[UPISystem]: Registration failed: Phone number already registered.",
                                "[UPISystem]: Registration failed: Generated UPI ID already in use. Try a different name.",
                                "[UPISystem]: Registration failed: Phone number already registered."},
              "Duplicate phone / duplicate generated UPI ID (phone checked first)");
      { ofstream clear("./upi_log.txt", ios::trunc); }
      Account* a = nullptr; Account* b = nullptr;
      try { a = u.login("Bob"); } catch (...) {}
      try { b = u.login("5555555555"); } catch (...) {}
      test(a == nullptr && b == nullptr, "Failed registrations create no account", "A rejected registration still created an account");
    }
    remove("./upi_log.txt");
    { UPISystem u;
      try { u.registerUser("1", "A"); u.registerUser("2", "B"); u.registerUser("3", "C"); } catch (...) {}
      Account* c = nullptr; try { c = u.login("3"); } catch (...) {}
      test(c && c->getName() == "C" && c->getUpiId() == "c@ok.iiitb", "Several users stored; correct one found", "Lookup after several registrations failed");
    }
    remove("./upi_log.txt");
    return 0;
}
