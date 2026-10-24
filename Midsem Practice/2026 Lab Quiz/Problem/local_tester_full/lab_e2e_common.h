#include "lab_util.h"
#define main app_original_main
#include "student.cpp"
#undef main

#include <fstream>
int main() {
    cout << "--- End-to-end run: " << SCEN << " (Sample_Runs.pdf + exit) ---" << endl;
    remove("./upi_log.txt");
    string in = readFile(string(SCEN) + ".in");
    runWithInput(in, []() { app_original_main(); });
    vector<string> exp = readLines(string(SCEN) + ".log.expected");
    testLog("./upi_log.txt", exp, string(SCEN) + ": whole log file matches; program exits");
    remove("./upi_log.txt");
    return 0;
}
