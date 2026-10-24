// Optional: interactive driver.  From the folder with skeleton.cpp:
//   cp skeleton.cpp local_tester/student.cpp && g++ -std=c++17 local_tester/run_driver.cpp -o drv && ./drv < local_tester/sample1.in
#include <iostream>
#include <string>
using namespace std;
#define main app_original_main
#include "student.cpp"
#undef main
#include "driver.h"
int main() { runDriver(cin); return 0; }
